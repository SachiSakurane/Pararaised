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

#if IPLUG_DSP // http://bit.ly/2S64BDd
  void OnReset() override;
  void OnActivate(bool active) override;
  void ProcessBlock(sample **inputs, sample **outputs, int nFrames) override;
#endif

private:
  module::store<sample> store;
  module::action<decltype(store), sample> action{store};

  bool is_2ch{false};
};
