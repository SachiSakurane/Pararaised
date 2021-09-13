#pragma once

#include <riw/rxcpp.hpp>

#include "IPlug_include_in_plug_hdr.h"
#include "src/module/store.hpp"
#include "src/module/action.hpp"

using namespace iplug;
using namespace igraphics;

class PLUG_CLASS_NAME final : public Plugin
{
public:
  PLUG_CLASS_NAME(const InstanceInfo &info);

  void OnParamChange(int paramIdx) override;

#if IPLUG_DSP // http://bit.ly/2S64BDd
  void ProcessBlock(sample **inputs, sample **outputs, int nFrames) override;
  void OnReset() override;
#endif

private:
  module::store<sample> store;
  module::action<decltype(store), sample> action{store};
};
