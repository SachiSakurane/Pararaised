#pragma once

#include <riw/utility/noncopyable.hpp>

#include "dsp/store.hpp"

namespace module
{
  template <class... StoreTypes>
  struct base_store : public StoreTypes...
  {
  };

  template <class SampleType>
  using store = base_store<dsp::store<SampleType>>;
}
