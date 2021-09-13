#pragma once

#include <riw/utility/noncopyable.hpp>
#include <rxcpp/rx.hpp>

#include "../../type/unit.hpp"

namespace module::dsp
{
  template <class SampleType>
  struct store : private riw::noncopyable<store<SampleType>>
  {
    const rxcpp::subjects::behavior<type::normalized<SampleType>> gain{static_cast<SampleType>(0)};
  };
}
