#pragma once

#include <functional>

#include <riw/utility/noncopyable.hpp>

#include "../../type/parameter.hpp"

namespace module::processor
{
  template <class SampleType>
  struct store : private riw::noncopyable<store<SampleType>>
  {
    template <class Injector>
    explicit store(const Injector &injector)
        : gain{injector, type::parameters::kGain, "Gain", 75., 0., 100.0, 0.01, "%"},
          pan{injector, type::parameters::kPan, "Pan", 0., -1., 1., 0.01} {}

    type::parameter<SampleType> gain;
    type::parameter<SampleType> pan;
  };
}
