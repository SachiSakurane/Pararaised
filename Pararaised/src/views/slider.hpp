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
  template <class SliderListener>
  concept slider_listener = requires(const SliderListener &listener)
  {
    typename SliderListener::value_type;
    {
      listener.normalized()
      } -> std::same_as<typename SliderListener::value_type>;
    listener.set_normalized(std::declval<typename SliderListener::value_type>());
    listener.send();
  };

  // knob ないやつは progress かも
  template <slider_listener ListenerType>
  class slider final : public usagi::wrapper::icontrol::iplug_traits::base_view_type
  {
    using traits_type = usagi::wrapper::icontrol::iplug_traits;
    const float kKnobSize = 6.f;

  public:
    using rect_type = traits_type::base_view_type::rect_type;
    using draw_context_type = traits_type::base_view_type::draw_context_type;

    slider(const usagi::concepts::geometry::rect_concept auto &frame, const ListenerType &l) : traits_type::base_view_type{frame}, listener{l}
    {
    }

    traits_type::value_type get_proportion() { return listener.normalized(); }

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

    // TODO: reset
    void event(typename mouse_traits::on_double_click_type mouse) override
    {
    }

  private:
    const ListenerType &listener;
    bool is_click{false};

    void position_to_proportion(traits_type::value_type p)
    {
      auto new_proportion = std::clamp((p - frame().l()) / frame().size().width(), static_cast<traits_type::value_type>(0), static_cast<traits_type::value_type>(1));
      listener.set_normalized(new_proportion);
      listener.send();
    }
  };

  template <slider_listener ListenerType>
  slider(const usagi::concepts::geometry::rect_concept auto &, const ListenerType &) -> slider<ListenerType>;
}
