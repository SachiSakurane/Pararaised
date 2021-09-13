#include "Pararaised.h"
#include "IPlug_include_in_plug_src.h"
#include "IControls.h"

#include "src/controls/sandbox.hpp"
#include "src/controls/anime_test.hpp"

PLUG_CLASS_NAME::PLUG_CLASS_NAME(const InstanceInfo &info)
    : Plugin(info, MakeConfig(kNumParams, 0))
{
  GetParam(kGain)->InitDouble("Gain", 0., 0., 100.0, 0.01, "%");

  store.gain.get_observable().distinct_until_changed().subscribe(
      [&](auto v)
      {
        GetParam(kGain)->SetNormalized(v);
        SendParameterValueFromUI(kGain, v);
      }) |
      riw::disposed(bag);

#if IPLUG_EDITOR // http://bit.ly/2S64BDd
  mMakeGraphicsFunc = [&]()
  {
    return MakeGraphics(*this, PLUG_WIDTH, PLUG_HEIGHT, PLUG_FPS, GetScaleForScreen(PLUG_WIDTH, PLUG_HEIGHT));
  };

  mLayoutFunc = [&](IGraphics *pGraphics)
  {
    pGraphics->AttachCornerResizer(EUIResizerMode::Scale, false);
    pGraphics->AttachPanelBackground(COLOR_GRAY);
    pGraphics->LoadFont("Roboto-Regular", ROBOTO_FN);

    pGraphics->EnableMouseOver(true);

    const IRECT b = pGraphics->GetBounds();
    pGraphics->AttachControl(new controls::sandbox{action, b});
    // pGraphics->AttachControl(new controls::box{b});
  };
#endif
}

void PLUG_CLASS_NAME::OnParamChange(int paramIdx)
{
  action.update_gain(GetParam(kGain)->GetNormalized());
}

void PLUG_CLASS_NAME::OnParamChangeUI(int paramIdx, EParamSource source)
{
  action.update_gain(GetParam(kGain)->GetNormalized());
}

#if IPLUG_DSP
void PLUG_CLASS_NAME::ProcessBlock(sample **inputs, sample **outputs, int frames)
{
  const double gain = GetParam(kGain)->Value() / 100.;
  const int channels = NOutChansConnected();

  for (int s = 0; s < frames; s++)
  {
    for (int c = 0; c < channels; c++)
    {
      outputs[c][s] = inputs[c][s] * gain;
    }
  }
}

void PLUG_CLASS_NAME::OnReset()
{
}
#endif
