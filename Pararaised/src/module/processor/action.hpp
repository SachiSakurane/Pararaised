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

    const type::parameter<SampleType>& get_gain() const { return store.gain; }

  private:
    const StoreType &store;
  };
}
