#pragma once

#include <iostream>
#include <vector>

#include <riw/rxcpp.hpp>
#include <riw/utility/noncopyable.hpp>

#include "../../type/unit.hpp"

namespace module::dsp
{
  enum parameters : int
  {
    kGain = 0,
    kNumParameters
  };

  template <class StoreType, class SampleType>
  class action : private riw::noncopyable<action<StoreType, SampleType>>
  {
  public:
    explicit action(const StoreType &s) : store{s}
    {
      store.gain.get_observable().subscribe([&memo = this->memo_gain](auto v)
                                            { memo = v; }) |
          riw::disposed(bag);
    }

    void set_parameter(std::function<void(module::dsp::parameters, type::normalized<SampleType>)> func)
    {
      store.gain.get_observable().distinct_until_changed().subscribe(
          [func](auto gain)
          {
            func(parameters::kGain, gain);
          }) |
          riw::disposed(bag);
    }

    void update_parameter(int index, type::normalized<SampleType> normalized)
    {
      switch (index)
      {
      case parameters::kGain:
        memo_gain = normalized;
        break;
      default:
        break;
      }
    }

    void update_gain(type::normalized<SampleType> v) const
    {
      store.gain.get_subscriber().on_next(v);
    }

    decltype(auto) gain() const { return memo_gain; }

  private:
    riw::dispose_bag bag;
    const StoreType &store;
    type::normalized<SampleType> memo_gain{store.gain.get_value()};
  };
}
