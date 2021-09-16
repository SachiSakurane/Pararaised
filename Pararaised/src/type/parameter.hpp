#pragma once

#include <functional>

#include <riw/utility/noncopyable.hpp>
#include <usagi/utility/floating_point.hpp>

#include "unit.hpp"

namespace type
{
  enum parameters : int
  {
    kGain = 0,
    kPan,
    kNumParameters
  };

  struct parameter_base : private riw::noncopyable<parameter_base>
  {
    template <class Injector>
    explicit parameter_base(const Injector &injector, type::parameters i)
        : index{i},
          get_param{std::bind(injector.get_iparam, index)},
          sender{std::bind(injector.send_parameter, index, std::placeholders::_1)}
    {
      assert(get_param);
      assert(sender);
    }

    const type::parameters index;

    iplug::IParam *get_iparam() const { return get_param(); }
    double raw_value() const { return get_iparam()->Value(); }
    type::normalized<double> normalized() const { return get_iparam()->GetNormalized(); }
    void raw_set(double v) const { setter(get_iparam(), v); }
    void set_normalized(type::normalized<double> v) const { normalized_setter(get_iparam(), v); }
    void send() const { sender(normalized()); }
    void reset() const { get_iparam()->SetToDefault(); }

  private:
    const std::function<iplug::IParam *()> get_param;
    const std::function<void(type::normalized<double>)> sender;
    const std::function<void(iplug::IParam *, double)> setter{[](iplug::IParam *param, double v)
                                                              { param->Set(v); }};
    const std::function<void(iplug::IParam *, type::normalized<double>)> normalized_setter{[](iplug::IParam *param, type::normalized<double> v)
                                                                                           { param->SetNormalized(static_cast<double>(v)); }};
  };

  template <class ValueType>
  class parameter
  {
  };

  template <usagi::utility::floating_point FloatType>
  struct parameter<FloatType> : parameter_base
  {
    using value_type = FloatType;

    template <class Injector, class... Args>
    explicit parameter<value_type>(const Injector &injector, type::parameters i, Args &&...args) : parameter_base{injector, i}
    {
      get_iparam()->InitDouble(std::forward<Args>(args)...);
    }

    value_type value() const { return static_cast<value_type>(parameter_base::raw_value()); }
  };
}
