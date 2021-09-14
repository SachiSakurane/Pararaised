#pragma once

#include <functional>

#include "../type/parameters.hpp"

#include "IControls.h"

namespace module
{
  struct injector
  {
    std::function<iplug::IParam *(type::parameters)> get_iparam;
  };
}
