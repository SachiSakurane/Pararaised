#pragma once

#include <iostream>

#include <usagi/wrapper/icontrol/view_wrapper.hpp>

#include "SkPath.h"

namespace views
{
  class slider final : public usagi::wrapper::icontrol::iplug_traits::base_view_type
  {
    using base_type = usagi::wrapper::icontrol::iplug_traits::base_view_type;

  public:
    using rect_type = base_type::rect_type;
    using draw_context_type = base_type::draw_context_type;

    slider(const rect_type &frame) : base_type{frame} {}

    void draw(draw_context_type &context) override
    {
      context.draw(
          [this](auto &g)
          {
            auto f = frame();
            SkPaint paint;
            paint.setAntiAlias(true);
            paint.setStyle(SkPaint::kStroke_Style);
            paint.setStrokeWidth(20);
            paint.setStrokeCap(SkPaint::kRound_Cap);
            paint.setColor(SK_ColorCYAN);

            std::cout << "{" << f.l() << "," << f.t() << "," << f.r() << "," << f.b() << "}" << std::endl;

            SkCanvas *canvas = static_cast<SkCanvas *>(g.GetDrawContext());
            SkPath path = SkPath::Line({f.l(), 100}, {f.r(), 100});

            canvas->drawPath(path, paint);
          });
    }

  private:
  };
} // namespace view
