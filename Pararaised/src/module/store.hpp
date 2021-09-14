#pragma once

#include <riw/utility/noncopyable.hpp>

#include "dsp/store.hpp"
#include "injector.hpp"

namespace module
{
  template <class... StoreTypes>
  struct base_store : public StoreTypes...
  {
    explicit base_store(module::injector injector) : StoreTypes{injector}... {}
  };

  template <class SampleType>
  using store = base_store<dsp::store<SampleType>>;
}
