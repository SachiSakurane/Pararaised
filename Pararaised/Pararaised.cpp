#include "Pararaised.h"
#include "IPlug_include_in_plug_src.h"
#include "IControls.h"

#include "src/controls/sandbox.hpp"

PLUG_CLASS_NAME::PLUG_CLASS_NAME(const InstanceInfo &info)
    : Plugin(info, MakeConfig(kNumParams, 0))
{
  GetParam(kGain)->InitDouble("Gain", 0., 0., 100.0, 0.01, "%");

#if IPLUG_EDITOR // http://bit.ly/2S64BDd
  mMakeGraphicsFunc = [&]() {
    return MakeGraphics(*this, PLUG_WIDTH, PLUG_HEIGHT, PLUG_FPS, GetScaleForScreen(PLUG_WIDTH, PLUG_HEIGHT));
  };
  
  mLayoutFunc = [&](IGraphics* pGraphics) {
    pGraphics->AttachCornerResizer(EUIResizerMode::Scale, false);
    pGraphics->AttachPanelBackground(COLOR_GRAY);
    pGraphics->LoadFont("Roboto-Regular", ROBOTO_FN);

    const IRECT b = pGraphics->GetBounds();
    pGraphics->AttachControl(new controls::sandbox{b});
  };
#endif
}

#if IPLUG_DSP
void PLUG_CLASS_NAME::ProcessBlock(sample **inputs, sample **outputs, int nFrames)
{
  const double gain = GetParam(kGain)->Value() / 100.;
  const int nChans = NOutChansConnected();
  
  for (int s = 0; s < nFrames; s++) {
    for (int c = 0; c < nChans; c++) {
      outputs[c][s] = inputs[c][s] * gain;
    }
  }
}
#endif
