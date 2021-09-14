#pragma once

#include <riw/utility/noncopyable.hpp>

#include "processor/store.hpp"
#include "injector.hpp"

namespace module
{
  template <class... StoreTypes>
  struct base_store : public StoreTypes...
  {
    explicit base_store(const module::injector& injector) : StoreTypes{injector}... {}
  };

  template <class SampleType>
  using store = base_store<processor::store<SampleType>>;
}
