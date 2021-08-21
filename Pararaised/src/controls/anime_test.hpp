#pragma once

#include <chrono>

#include <riw/utility/noncopyable.hpp>
#include <usagi/geometry.hpp>
#include <usagi/ui/gesture.hpp>
#include <usagi/ui/surface.hpp>
#include <usagi/wrapper/icontrol/view_wrapper.hpp>

#include "../utility/animation.hpp"

namespace controls
{
  class anime_test : public usagi::wrapper::icontrol::view_wrapper, private riw::noncopyable<anime_test>
  {
    using traits_type = usagi::wrapper::icontrol::iplug_traits;

  public:
    explicit anime_test(const IRECT &bounds) : usagi::wrapper::icontrol::view_wrapper{bounds}
    {
      auto f = local_view.frame();
      auto padding = usagi::geometry::padding(f, 16.f);

      local_view.add_sub_view(
          traits_type::base_view_type{usagi::geometry::from_top(padding, 100.f)} |
          usagi::ui::surfaced(
              [&animator = this->animator](auto &context, const auto &v)
              {
                SkAutoCanvasRestore restore{&context, true};
                context.saveLayer(nullptr, nullptr);

                auto frame = v.frame();

                SkPaint paint;
                paint.setColor(SK_ColorCYAN);
                context.drawRect(SkRect::MakeXYWH(frame.l() + (frame.size().width() - 100.f) * animator.get_animation(), frame.t(), 100.f, 100.f), paint);
              }));

      local_view.add_sub_view(
          traits_type::base_view_type{usagi::geometry::from_bottom(padding, 100.f)} |
          usagi::ui::surfaced(
              [&animator = this->animator](auto &context, const auto &v)
              {
                SkAutoCanvasRestore restore{&context, true};
                context.saveLayer(nullptr, nullptr);

                SkPaint paint;
                paint.setColor(SK_ColorYELLOW);
                context.drawRect(usagi::wrapper::skia::to_rect(v.frame()), paint);
              }) |
          usagi::ui::gestured(
              [&animator = this->animator](traits_type::mouse_traits::on_down_type mouse, auto &v)
              {
                if (usagi::geometry::contain(v.frame(), traits_type::point_type{mouse.x, mouse.y}))
                  animator.set_flag(!animator.get_flag());
              }));
    }

  private:
    std::unique_ptr<Timer> timer{
        std::unique_ptr<Timer>(Timer::Create(
            [&](Timer &t)
            { SetDirty(false); },
            16))};

    animation animator{std::chrono::milliseconds{1000}};
  };
}
