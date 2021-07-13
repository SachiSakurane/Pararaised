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
      context.drawRect(usagi::wrapper::skia::to_rect(usagi::geometry::reduce_from_right(f, normalized_value() * width)), src);

      // knob

    }

  private:
  };
}
