#include "Pararaised.h"
#include "IPlug_include_in_plug_src.h"
#include "IControls.h"

#include "src/controls/anime_test.hpp"
#include "src/controls/layered_test.hpp"
#include "src/controls/sandbox.hpp"

#include "src/module/injector.hpp"

PLUG_CLASS_NAME::PLUG_CLASS_NAME(const InstanceInfo &info)
    : Plugin(info, MakeConfig(type::parameters::kNumParameters, 0)),
      store{module::injector{
          .send_parameter = [&](type::parameters index, type::normalized<double> normalized)
          { SendParameterValueFromUI(index, normalized); },
          .get_iparam = [&](type::parameters index)
          { return GetParam(index); }}}
{
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
    // pGraphics->AttachControl(new controls::layered_test{b});
    // pGraphics->AttachControl(new controls::anime_test{b});
  };
#endif
}

#if IPLUG_DSP
void PLUG_CLASS_NAME::OnReset()
{
  std::cout << "on reset" << std::endl;
  is_2ch = NOutChansConnected() == 2;
}

void PLUG_CLASS_NAME::OnActivate(bool active)
{
  std::cout << "on active" << std::endl;
}

void PLUG_CLASS_NAME::ProcessBlock(sample **inputs, sample **outputs, int frames)
{
  const double gain = GetParam(type::parameters::kGain)->Value() / 100.;

  if (is_2ch)
  {
    const double pan = GetParam(type::parameters::kPan)->Value();
    for (int s = 0; s < frames; s++)
    {
      outputs[0][s] = inputs[0][s] * gain * std::clamp(1. - pan, 0., 1.);
      outputs[1][s] = inputs[1][s] * gain * std::clamp(1. + pan, 0., 1.);
    }
  }
  else
  {
    for (int s = 0; s < frames; s++)
    {
      for (int c = 0; c < NOutChansConnected(); c++)
      {
        outputs[c][s] = inputs[c][s] * gain;
      }
    }
  }
}
#endif
