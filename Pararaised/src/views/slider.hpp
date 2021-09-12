#pragma once

#include <iostream>
#include <algorithm>

#include <riw/algorithm/inverse.hpp>
#include <riw/rxcpp.hpp>
#include <usagi/concepts.hpp>
#include <usagi/geometry.hpp>
#include <usagi/wrapper/icontrol/view_wrapper.hpp>
#include <usagi/wrapper/skia/rect_wrapper.hpp>

#include "SkPath.h"

namespace views
{
  // knob ないやつは progress かも
  class slider final : public usagi::wrapper::icontrol::iplug_traits::base_view_type
  {
    using traits_type = usagi::wrapper::icontrol::iplug_traits;
    const float kKnobSize = 6.f;

  public:
    using rect_type = traits_type::base_view_type::rect_type;
    using draw_context_type = traits_type::base_view_type::draw_context_type;

    slider(const usagi::concepts::geometry::rect_concept auto &frame, std::function<void(traits_type::value_type)> observe) : traits_type::base_view_type{frame} {
      proportion.get_observable().subscribe(observe) | riw::disposed(bag);
    }

    traits_type::value_type get_proportion() { return proportion.get_value(); }

    void draw(draw_context_type &context) override
    {
      SkAutoCanvasRestore restore{&context, true};
      context.saveLayer(nullptr, nullptr);

      auto f = frame();
      auto [width, height] = f.size()();
      SkPaint src, dst;

      context.clear(SK_ColorTRANSPARENT);
      context.drawRRect(SkRRect::MakeRectXY(usagi::wrapper::skia::to_rect(f), height * 0.5f, height * 0.5f), dst);

      // background
      src.setBlendMode(SkBlendMode::kSrcIn);
      src.setColor(SK_ColorLTGRAY);
      context.drawPaint(src);

      // colored
      src.setColor(SK_ColorGRAY);
      context.drawRect(usagi::wrapper::skia::to_rect(usagi::geometry::reduce_from_right(f, riw::inverse(get_proportion()) * width)), src);

      // knob
      SkPaint knob;
      knob.setColor(SK_ColorWHITE);
      context.drawCircle({f.l() + get_proportion() * width, f.center().y()}, kKnobSize, knob);
    }

    void event(typename mouse_traits::on_down_type mouse) override
    {
      if (usagi::geometry::contain(
              usagi::geometry::from_height(frame(), kKnobSize * 4.f),
              point_type{mouse.x, mouse.y}))
      {
        is_click = true;
        position_to_proportion(mouse.x);
        if (auto g = mouse.graphics)
        {
          g->HideMouseCursor(true, false);
        }
      }
    }

    void event(typename mouse_traits::on_drag_type mouse) override
    {
      if (is_click)
      {
        position_to_proportion(mouse.x);
      }
    }

    void event(typename mouse_traits::on_up_type mouse) override
    {
      if (is_click)
      {
        position_to_proportion(mouse.x);
        is_click = false;
        if (auto g = mouse.graphics)
        {
          float p = get_proportion();
          auto f = frame();
          g->MoveMouseCursor(p * f.size().width() + f.l(), f.center().y());
          g->HideMouseCursor(false);
          g->SetMouseCursor(ECursor::HAND);
        }
      }
    }

    void event(typename mouse_traits::on_over_type mouse) override
    {
      if (auto g = mouse.graphics)
      {
        if (usagi::geometry::contain(
                usagi::geometry::from_height(frame(), kKnobSize * 4.f),
                point_type{mouse.x, mouse.y}))
        {
          g->SetMouseCursor(ECursor::HAND);
        }
        else
        {
          g->SetMouseCursor();
        }
      }
    }

    void event(typename mouse_traits::on_out_type mouse) override
    {
      if (auto g = mouse.graphics)
      {
        g->SetMouseCursor();
      }
    }

    const rxcpp::subjects::behavior<traits_type::value_type> proportion{0.0f};
  private:
    riw::dispose_bag bag;
    bool is_click{false};

    void position_to_proportion(traits_type::value_type p)
    {
      auto new_proportion = std::clamp((p - frame().l()) / frame().size().width(), static_cast<traits_type::value_type>(0), static_cast<traits_type::value_type>(1));
      proportion.get_subscriber().on_next(new_proportion);
    }
  };
}
