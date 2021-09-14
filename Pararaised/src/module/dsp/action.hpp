#pragma once

#include <iostream>
#include <vector>

#include <riw/rxcpp.hpp>
#include <riw/utility/noncopyable.hpp>
#include <riw/utility/nonmovable.hpp>

#include "../../type/unit.hpp"

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
    using value_type = ValueType;

    explicit memo(const rxcpp::subjects::behavior<value_type> &behavior)
        : memo_value{behavior.get_value()}, observable{behavior.get_observable()}, subscriber{behavior.get_subscriber()}
    {
      observable.subscribe([&](auto v)
                           { write_memo(v); }) |
          riw::disposed(bag);
    }

    value_type operator()() const
    {
      return memo_value;
    }

    void update(value_type v) const { subscriber.on_next(v); }
    void write_memo(value_type v) { memo_value = v; }

    const rxcpp::observable<value_type> observable;

  private:
    riw::dispose_bag bag;
    value_type memo_value;
    const rxcpp::subscriber<value_type> subscriber;
  };

  template <class StoreType, class SampleType>
  class action : private riw::noncopyable<action<StoreType, SampleType>>
  {
  public:
    explicit action(const StoreType &s) : store{s}
    {
    }

    void set_parameter(std::function<void(module::dsp::parameters, type::normalized<SampleType>)> func)
    {
      memo_gain.observable.distinct_until_changed().subscribe(
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
        memo_gain.write_memo(normalized);
        break;
      default:
        break;
      }
    }

    const memo<type::normalized<SampleType>> &gain() const { return memo_gain; }

  private:
    riw::dispose_bag bag;
    const StoreType &store;
    memo<type::normalized<SampleType>> memo_gain{store.gain};
  };
}
