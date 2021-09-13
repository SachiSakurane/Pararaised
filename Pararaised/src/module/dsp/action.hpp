#pragma once

#include <iostream>
#include <vector>

#include <riw/rxcpp.hpp>
#include <riw/utility/noncopyable.hpp>

namespace module::dsp
{
  enum parameters : int
  {
    kGain = 0,
    kNumParameters
  };

  template <class ValueType>
  class memo
  {
  public:
    template <class Observable>
    explicit memo(Observable o)
    {
      o.subscribe([&](ValueType v)
                  { value = v; }) |
          riw::disposed(bag);
    }

    ValueType value;

  private:
    riw::dispose_bag bag;
  };

  template <class StoreType, class SampleType>
  class action : private riw::noncopyable<action<StoreType, SampleType>>
  {
  public:
    explicit action(const StoreType &s) : store{s} {}

    void set_parameter(std::function<void(module::dsp::parameters, double)> func)
    {
      store.gain.get_observable().distinct_until_changed().subscribe(
          [func](auto gain)
          { func(parameters::kGain, gain); }) |
          riw::disposed(bag);
    }

    void update_parameter(int index, double value)
    {
      switch (index)
      {
      case parameters::kGain:
        memo_gain.value = value;
        break;
      default:
        break;
      }
    }

    void update_gain(SampleType v) const
    {
      store.gain.get_subscriber().on_next(v);
    }

    decltype(auto) gain() const { return memo_gain.value; }

  private:
    riw::dispose_bag bag;
    const StoreType &store;
    memo<SampleType> memo_gain{store.gain.get_observable()};
  };
}
