#pragma once

#include <functional>

#include <riw/utility/noncopyable.hpp>

// dsp は process block でも使用される想定
namespace module::dsp
{
  struct store : private riw::noncopyable<store>
  {
    bool is_bypassed{};
    double sample_rate{};
    size_t block_size{};
    size_t channels{};
    bool is_2ch{false};
  };
}
