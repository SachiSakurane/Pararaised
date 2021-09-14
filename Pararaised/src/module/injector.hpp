#pragma once

#include <functional>

#include "../type/parameter.hpp"
#include "../type/unit.hpp"

#include "IControls.h"

namespace module
{
  struct injector
  {
    const std::function<void(type::parameters, type::normalized<double>)> send_parameter;
    const std::function<iplug::IParam *(type::parameters)> get_iparam;
  };
}
