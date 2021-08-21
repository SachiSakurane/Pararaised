#pragma once

#include <cmath>
#include <chrono>

#include <riw/easing.hpp>

class animation
{
public:
  explicit animation(const std::chrono::milliseconds dur) : duration{dur}
  {
  }

  const std::chrono::milliseconds duration;

  bool is_animated() const
  {
    return duration > (std::chrono::steady_clock::now() - update_time);
  }

  bool get_flag() const { return flag; }

  void set_flag(bool new_flag)
  {
    if (new_flag != flag)
    {
      last = get_animation();
      flag = !flag;
      update_time = std::chrono::steady_clock::now();
    }
  }

  template <class Easing = riw::quint>
  float get_animation() const
  {
    auto d = std::chrono::steady_clock::now() - update_time;
    if (duration > d)
    {
      auto v = static_cast<float>(std::chrono::duration_cast<std::chrono::milliseconds>(d).count()) / static_cast<float>(duration.count());
      return flag ? std::lerp(last, 1.f, riw::ease_out<Easing>(v))
                  : std::lerp(0.f, last, riw::ease_in<Easing>(1.f - v));
    }
    return flag ? 1.f : 0.f;
  }

  void reset()
  {
    flag = false;
    last = 0;
    update_time = std::chrono::steady_clock::now();
  }

private:
  std::chrono::steady_clock::time_point update_time{std::chrono::steady_clock::now()};
  bool flag{false};
  float last{0};
};
