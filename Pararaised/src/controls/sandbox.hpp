#pragma once

#include <riw/utility/noncopyable.h>
#include <usagi/geometry.hpp>
#include <usagi/ui/surface.hpp>
#include <usagi/wrapper/icontrol/view_wrapper.hpp>
#include <usagi/wrapper/skia/rect_wrapper.hpp>

#include "../views/slider.hpp"

namespace controls
{
  class sandbox : public usagi::wrapper::icontrol::view_wrapper, private riw::noncopyable<sandbox>
  {
  public:
    sandbox(const IRECT &bounds) : usagi::wrapper::icontrol::view_wrapper{bounds}
    {
      timer = std::unique_ptr<Timer>(Timer::Create(
          [&](Timer &t)
          { SetDirty(false); },
          16));

      auto f = local_view.frame(); 
      auto padding = usagi::geometry::padding(f, 16.f);

      local_view.add_sub_view(
          usagi::ui::surfaced<usagi::wrapper::icontrol::iplug_traits::base_view_type>(
              [](auto &context, const auto &v)
              {
                SkAutoCanvasRestore restore{&context, true};
                context.saveLayer(nullptr, nullptr);

                SkPaint paint;
                paint.setColor(SK_ColorCYAN);
                context.drawRect(usagi::wrapper::skia::to_rect(v.frame()), paint);
              },
              padding));

      local_view.add_sub_view(views::slider{
          usagi::geometry::from_top(usagi::geometry::padding(padding, 16.f), 4.f)});
    }

  private:
    std::unique_ptr<Timer> timer;
  };
}
