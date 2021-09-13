#pragma once

#include <riw/utility/noncopyable.hpp>
#include <rxcpp/rx.hpp>

namespace module::dsp
{
  template <class SampleType>
  struct store : private riw::noncopyable<store<SampleType>>
  {
    const rxcpp::subjects::behavior<SampleType> gain{static_cast<SampleType>(0)};
  };
}
