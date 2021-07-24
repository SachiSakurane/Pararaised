#pragma once

#include <riw/utility/noncopyable.h>
#include <usagi/geometry.hpp>
#include <usagi/ui/surface.hpp>
#include <usagi/wrapper/icontrol/view_wrapper.hpp>

namespace controls
{
  class tiles : public usagi::wrapper::icontrol::view_wrapper, private riw::noncopyable<tiles>
  {
    using traits_type = usagi::wrapper::icontrol::iplug_traits;

    class tile : public traits_type::base_view_type
    {
    public:
      tile(traits_type::rect_type &&rect) : traits_type::base_view_type{rect}
      {
      }

    private:
    };

  public:
    tiles(const IRECT &bounds) : usagi::wrapper::icontrol::view_wrapper{bounds}
    {
      timer = std::unique_ptr<Timer>(Timer::Create(
          [&](Timer &t)
          { SetDirty(false); },
          16));

      auto f = local_view.frame();
      auto padding = usagi::geometry::padding(f, 16.f);

      local_view.add_sub_view(
          traits_type::base_view_type{padding} |
          usagi::ui::surfaced(
              [](auto &context, const auto &v)
              {
                SkAutoCanvasRestore restore{&context, true};
                context.saveLayer(nullptr, nullptr);

                SkPaint paint;
                paint.setColor(SK_ColorCYAN);
                context.drawRect(usagi::wrapper::skia::to_rect(v.frame()), paint);
              }));
    }

  private:
    std::unique_ptr<Timer> timer;
  };
}
