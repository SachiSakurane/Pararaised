#pragma once

#include <riw/utility/noncopyable.hpp>

namespace module::processor
{
  template <class StoreType, class SampleType>
  class action : private riw::noncopyable<action<StoreType, SampleType>>
  {
  public:
    explicit action(const StoreType &s) : store{s}
    {
    }

    const auto &get_gain() const { return store.gain; }
    const auto &get_pan() const { return store.pan; }

  private:
    const StoreType &store;
  };
}
