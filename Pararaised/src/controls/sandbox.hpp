#pragma once

#include <riw/utility/noncopyable.hpp>
#include <usagi/geometry.hpp>
#include <usagi/ui/gesture.hpp>
#include <usagi/ui/surface.hpp>
#include <usagi/wrapper/icontrol/view_wrapper.hpp>
#include <usagi/wrapper/skia/rect_wrapper.hpp>

#include "../utility/animation.hpp"
#include "../views/slider.hpp"

namespace controls
{
  template <class ActionType>
  class sandbox : public usagi::wrapper::icontrol::view_wrapper, private riw::noncopyable<sandbox<ActionType>>
  {
    using traits_type = usagi::wrapper::icontrol::iplug_traits;

  public:
    sandbox(const ActionType &a, const IRECT &bounds) : action{a}, usagi::wrapper::icontrol::view_wrapper{bounds}
    {
      {
        auto &view = local_view.add_sub_view(
            traits_type::base_view_type{usagi::geometry::padding(local_view.frame(), 16.f)} |
            usagi::ui::surfaced(
                [](auto &context, const auto &v)
                {
                  SkAutoCanvasRestore restore{&context, true};
                  context.saveLayer(nullptr, nullptr);

                  SkPaint paint;
                  paint.setColor(SK_ColorBLUE);
                  context.drawRect(usagi::wrapper::skia::to_rect(v.frame()), paint);
                }) /* |
          usagi::ui::gestured(
              [](traits_type::mouse_traits::on_down_type mouse, auto &v)
              {
                if (usagi::geometry::contain(v.frame(), traits_type::point_type{mouse.x, mouse.y}))
                  std::cout << "tapped" << std::endl;
              })*/
        );
        auto f = usagi::geometry::padding(view.frame(), 16.f);
        auto [x, ff] = usagi::geometry::reduce_from_top(f, 4.f);
        view.add_sub_view(views::slider{x, action.get_gain()});

        auto [o, fff] = usagi::geometry::reduce_from_top(ff, 32.f);
        auto [xx, oo] = usagi::geometry::reduce_from_top(fff, 4.f);
        view.add_sub_view(views::slider{xx, action.get_pan()});
      }
      local_view.add_sub_view(
          traits_type::base_view_type{usagi::geometry::from_top(usagi::geometry::padding(local_view.frame(), 16.f), 100.f)} |
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
          traits_type::base_view_type{usagi::geometry::from_bottom(usagi::geometry::padding(local_view.frame(), 16.f), 100.f)} |
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

    const ActionType &action;
    animation animator{std::chrono::milliseconds{1000}};
  };

  template <class ActionType>
  sandbox(const ActionType &, const IRECT &) -> sandbox<ActionType>;
}
