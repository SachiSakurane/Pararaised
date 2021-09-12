#pragma once

#include <riw/rxcpp/disposed.hpp>
#include <riw/utility/noncopyable.hpp>

#include "dsp/action.hpp"

namespace module
{
  template <class StoreType, class... ActionTypes>
  class base_action : public ActionTypes...
  {
  public:
    explicit base_action(const StoreType &store) : ActionTypes{store}... {}

    // class bundler : public ActionTypes::template bundler<base_action>...
    // {
    // public:
    //   explicit bundler(const base_action &action) : ActionTypes::template bundler<base_action>{action}... {}
    // };
  };

  template <class StoreType, class SampleType>
  using action = base_action<StoreType, dsp::action<StoreType, SampleType>>;
}
