#pragma once

#include <iostream>

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
    using base_type = usagi::wrapper::icontrol::iplug_traits::base_view_type;

  public:
    using rect_type = base_type::rect_type;
    using draw_context_type = base_type::draw_context_type;

    slider(const usagi::concepts::geometry::rect_concept auto &frame) : base_type{frame} {}

    float normalized_value() { return 0.5f; }

    void draw(draw_context_type &context) override
    {
      context.draw(
          [this](auto &g)
          {
            auto f = frame();
            auto [width, height] = f.size()();
            SkPaint src, dst;
            SkCanvas *canvas = static_cast<SkCanvas *>(g.GetDrawContext());

            canvas->clear(SK_ColorTRANSPARENT);
            canvas->drawRRect(SkRRect::MakeRectXY(usagi::wrapper::skia::to_rect(f), height * 0.5f, height * 0.5f), dst);

            // background
            src.setBlendMode(SkBlendMode::kSrcIn);
            src.setColor(SK_ColorLTGRAY);
            canvas->drawPaint(src);

            // colored
            src.setColor(SK_ColorGRAY);
            canvas->drawRect(usagi::wrapper::skia::to_rect(usagi::geometry::reduce_from_right(f, normalized_value() * width)), src);
          });
    }

  private:
  };
}
