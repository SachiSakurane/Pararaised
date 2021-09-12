#pragma once

#include <vector>

#include <riw/utility/noncopyable.hpp>

namespace module::dsp
{
  template <class StoreType, class SampleType>
  class action : private riw::noncopyable<action<StoreType, SampleType>>
  {
  public:
    explicit action(const StoreType &s) : store{s}
    {
    }

    void update_gain(SampleType v) const
    {
      store.gain.get_subscriber().on_next(v);
    }

  private:
    const StoreType &store;
  };
}
