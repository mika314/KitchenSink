// (c) 2023 Mika Pi

#include "SettingsUi.h"
#include "Settings.h"
#include <Misc/ConfigUtilities.h>

auto USettingsUi::OnWidgetRebuilt() -> void
{
  auto settings = Cast<USettings>(UGameplayStatics::LoadGameFromSlot("settings", 0));
  if (!settings)
    settings = NewObject<USettings>(this, USettings::StaticClass());

  const auto videoQuality = static_cast<VideoQuality>(settings->videoQuality);

  setVideoQuality(videoQuality, false);

  {
    auto widget = getProp<UCheckBox>(this, TEXT("VideoQualityLow"));
    CHECK_RET(widget);
    widget->SetCheckedState(videoQuality == VideoQuality::low ? ECheckBoxState::Checked
                                                              : ECheckBoxState::Unchecked);
    widget->OnCheckStateChanged.AddDynamic(this, &USettingsUi::onVideoQualityLowChanged);
  }
  {
    auto widget = getProp<UCheckBox>(this, TEXT("VideoQualityMed"));
    CHECK_RET(widget);
    widget->SetCheckedState(videoQuality == VideoQuality::med ? ECheckBoxState::Checked
                                                              : ECheckBoxState::Unchecked);
    widget->OnCheckStateChanged.AddDynamic(this, &USettingsUi::onVideoQualityMedChanged);
  }
  {
    auto widget = getProp<UCheckBox>(this, TEXT("VideoQualityHigh"));
    CHECK_RET(widget);
    widget->SetCheckedState(videoQuality == VideoQuality::high ? ECheckBoxState::Checked
                                                               : ECheckBoxState::Unchecked);
    widget->OnCheckStateChanged.AddDynamic(this, &USettingsUi::onVideoQualityHighChanged);
  }
  {
    auto widget = getProp<UCheckBox>(this, TEXT("VideoQualityEpic"));
    CHECK_RET(widget);
    widget->SetCheckedState(videoQuality == VideoQuality::epic ? ECheckBoxState::Checked
                                                               : ECheckBoxState::Unchecked);
    widget->OnCheckStateChanged.AddDynamic(this, &USettingsUi::onVideoQualityEpicChanged);
  }
}

auto USettingsUi::onVideoQualityLowChanged(bool) -> void
{
  {
    auto widget = getProp<UCheckBox>(this, TEXT("VideoQualityLow"));
    CHECK_RET(widget);
    widget->SetCheckedState(ECheckBoxState::Checked);
  }
  {
    auto widget = getProp<UCheckBox>(this, TEXT("VideoQualityMed"));
    CHECK_RET(widget);
    widget->SetCheckedState(ECheckBoxState::Unchecked);
  }
  {
    auto widget = getProp<UCheckBox>(this, TEXT("VideoQualityHigh"));
    CHECK_RET(widget);
    widget->SetCheckedState(ECheckBoxState::Unchecked);
  }
  {
    auto widget = getProp<UCheckBox>(this, TEXT("VideoQualityEpic"));
    CHECK_RET(widget);
    widget->SetCheckedState(ECheckBoxState::Unchecked);
  }
  setVideoQuality(VideoQuality::low);
}

auto USettingsUi::onVideoQualityMedChanged(bool) -> void
{
  {
    auto widget = getProp<UCheckBox>(this, TEXT("VideoQualityLow"));
    CHECK_RET(widget);
    widget->SetCheckedState(ECheckBoxState::Unchecked);
  }
  {
    auto widget = getProp<UCheckBox>(this, TEXT("VideoQualityMed"));
    CHECK_RET(widget);
    widget->SetCheckedState(ECheckBoxState::Checked);
  }
  {
    auto widget = getProp<UCheckBox>(this, TEXT("VideoQualityHigh"));
    CHECK_RET(widget);
    widget->SetCheckedState(ECheckBoxState::Unchecked);
  }
  {
    auto widget = getProp<UCheckBox>(this, TEXT("VideoQualityEpic"));
    CHECK_RET(widget);
    widget->SetCheckedState(ECheckBoxState::Unchecked);
  }
  setVideoQuality(VideoQuality::med);
}

auto USettingsUi::onVideoQualityHighChanged(bool) -> void
{
  {
    auto widget = getProp<UCheckBox>(this, TEXT("VideoQualityLow"));
    CHECK_RET(widget);
    widget->SetCheckedState(ECheckBoxState::Unchecked);
  }
  {
    auto widget = getProp<UCheckBox>(this, TEXT("VideoQualityMed"));
    CHECK_RET(widget);
    widget->SetCheckedState(ECheckBoxState::Unchecked);
  }
  {
    auto widget = getProp<UCheckBox>(this, TEXT("VideoQualityHigh"));
    CHECK_RET(widget);
    widget->SetCheckedState(ECheckBoxState::Checked);
  }
  {
    auto widget = getProp<UCheckBox>(this, TEXT("VideoQualityEpic"));
    CHECK_RET(widget);
    widget->SetCheckedState(ECheckBoxState::Unchecked);
  }
  setVideoQuality(VideoQuality::high);
}

auto USettingsUi::onVideoQualityEpicChanged(bool) -> void
{
  {
    auto widget = getProp<UCheckBox>(this, TEXT("VideoQualityLow"));
    CHECK_RET(widget);
    widget->SetCheckedState(ECheckBoxState::Unchecked);
  }
  {
    auto widget = getProp<UCheckBox>(this, TEXT("VideoQualityMed"));
    CHECK_RET(widget);
    widget->SetCheckedState(ECheckBoxState::Unchecked);
  }
  {
    auto widget = getProp<UCheckBox>(this, TEXT("VideoQualityHigh"));
    CHECK_RET(widget);
    widget->SetCheckedState(ECheckBoxState::Unchecked);
  }
  {
    auto widget = getProp<UCheckBox>(this, TEXT("VideoQualityEpic"));
    CHECK_RET(widget);
    widget->SetCheckedState(ECheckBoxState::Checked);
  }
  setVideoQuality(VideoQuality::epic);
}

auto USettingsUi::setVideoQuality(VideoQuality v, bool save) -> void
{
  if (save)
  {
    auto settings = Cast<USettings>(UGameplayStatics::LoadGameFromSlot("settings", 0));
    if (!settings)
      settings = NewObject<USettings>(this, USettings::StaticClass());
    settings->videoQuality = static_cast<int>(v);
    UGameplayStatics::SaveGameToSlot(settings, "settings", 0);
  }

  auto controller = UGameplayStatics::GetPlayerController(this, 0);
  switch (v)
  {
  case VideoQuality::epic:
    UE::ConfigUtilities::ApplyCVarSettingsFromIni(
      TEXT("ViewDistanceQuality@3"), TEXT("Scalability"), ECVF_SetByScalability, false);
    UE::ConfigUtilities::ApplyCVarSettingsFromIni(
      TEXT("ViewDistanceQuality@3"), TEXT("Scalability"), ECVF_SetByScalability, false);
    UE::ConfigUtilities::ApplyCVarSettingsFromIni(
      TEXT("AntiAliasingQuality@3"), TEXT("Scalability"), ECVF_SetByScalability, false);
    UE::ConfigUtilities::ApplyCVarSettingsFromIni(
      TEXT("ShadowQuality@3"), TEXT("Scalability"), ECVF_SetByScalability, false);
    UE::ConfigUtilities::ApplyCVarSettingsFromIni(
      TEXT("GlobalIlluminationQuality@3"), TEXT("Scalability"), ECVF_SetByScalability, false);
    UE::ConfigUtilities::ApplyCVarSettingsFromIni(
      TEXT("ReflectionQuality@3"), TEXT("Scalability"), ECVF_SetByScalability, false);
    UE::ConfigUtilities::ApplyCVarSettingsFromIni(
      TEXT("PostProcessQuality@3"), TEXT("Scalability"), ECVF_SetByScalability, false);
    UE::ConfigUtilities::ApplyCVarSettingsFromIni(
      TEXT("TextureQuality@3"), TEXT("Scalability"), ECVF_SetByScalability, false);
    UE::ConfigUtilities::ApplyCVarSettingsFromIni(
      TEXT("EffectsQuality@3"), TEXT("Scalability"), ECVF_SetByScalability, false);
    UE::ConfigUtilities::ApplyCVarSettingsFromIni(
      TEXT("FoliageQuality@3"), TEXT("Scalability"), ECVF_SetByScalability, false);
    UE::ConfigUtilities::ApplyCVarSettingsFromIni(
      TEXT("ShadingQuality@3"), TEXT("Scalability"), ECVF_SetByScalability, false);
    break;
  case VideoQuality::high:
    UE::ConfigUtilities::ApplyCVarSettingsFromIni(
      TEXT("ViewDistanceQuality@2"), TEXT("Scalability"), ECVF_SetByScalability, false);
    UE::ConfigUtilities::ApplyCVarSettingsFromIni(
      TEXT("AntiAliasingQuality@2"), TEXT("Scalability"), ECVF_SetByScalability, false);
    UE::ConfigUtilities::ApplyCVarSettingsFromIni(
      TEXT("ShadowQuality@2"), TEXT("Scalability"), ECVF_SetByScalability, false);
    UE::ConfigUtilities::ApplyCVarSettingsFromIni(
      TEXT("GlobalIlluminationQuality@2"), TEXT("Scalability"), ECVF_SetByScalability, false);
    UE::ConfigUtilities::ApplyCVarSettingsFromIni(
      TEXT("ReflectionQuality@2"), TEXT("Scalability"), ECVF_SetByScalability, false);
    UE::ConfigUtilities::ApplyCVarSettingsFromIni(
      TEXT("PostProcessQuality@2"), TEXT("Scalability"), ECVF_SetByScalability, false);
    UE::ConfigUtilities::ApplyCVarSettingsFromIni(
      TEXT("TextureQuality@2"), TEXT("Scalability"), ECVF_SetByScalability, false);
    UE::ConfigUtilities::ApplyCVarSettingsFromIni(
      TEXT("EffectsQuality@2"), TEXT("Scalability"), ECVF_SetByScalability, false);
    UE::ConfigUtilities::ApplyCVarSettingsFromIni(
      TEXT("FoliageQuality@2"), TEXT("Scalability"), ECVF_SetByScalability, false);
    UE::ConfigUtilities::ApplyCVarSettingsFromIni(
      TEXT("ShadingQuality@2"), TEXT("Scalability"), ECVF_SetByScalability, false);
    break;
  case VideoQuality::med:
    UE::ConfigUtilities::ApplyCVarSettingsFromIni(
      TEXT("ViewDistanceQuality@1"), TEXT("Scalability"), ECVF_SetByScalability, false);
    UE::ConfigUtilities::ApplyCVarSettingsFromIni(
      TEXT("AntiAliasingQuality@1"), TEXT("Scalability"), ECVF_SetByScalability, false);
    UE::ConfigUtilities::ApplyCVarSettingsFromIni(
      TEXT("ShadowQuality@1"), TEXT("Scalability"), ECVF_SetByScalability, false);
    UE::ConfigUtilities::ApplyCVarSettingsFromIni(
      TEXT("GlobalIlluminationQuality@1"), TEXT("Scalability"), ECVF_SetByScalability, false);
    UE::ConfigUtilities::ApplyCVarSettingsFromIni(
      TEXT("ReflectionQuality@1"), TEXT("Scalability"), ECVF_SetByScalability, false);
    UE::ConfigUtilities::ApplyCVarSettingsFromIni(
      TEXT("PostProcessQuality@1"), TEXT("Scalability"), ECVF_SetByScalability, false);
    UE::ConfigUtilities::ApplyCVarSettingsFromIni(
      TEXT("TextureQuality@1"), TEXT("Scalability"), ECVF_SetByScalability, false);
    UE::ConfigUtilities::ApplyCVarSettingsFromIni(
      TEXT("EffectsQuality@1"), TEXT("Scalability"), ECVF_SetByScalability, false);
    UE::ConfigUtilities::ApplyCVarSettingsFromIni(
      TEXT("FoliageQuality@1"), TEXT("Scalability"), ECVF_SetByScalability, false);
    UE::ConfigUtilities::ApplyCVarSettingsFromIni(
      TEXT("ShadingQuality@1"), TEXT("Scalability"), ECVF_SetByScalability, false);
    break;
  case VideoQuality::low:
    UE::ConfigUtilities::ApplyCVarSettingsFromIni(
      TEXT("ViewDistanceQuality@0"), TEXT("Scalability"), ECVF_SetByScalability, false);
    UE::ConfigUtilities::ApplyCVarSettingsFromIni(
      TEXT("AntiAliasingQuality@0"), TEXT("Scalability"), ECVF_SetByScalability, false);
    UE::ConfigUtilities::ApplyCVarSettingsFromIni(
      TEXT("ShadowQuality@0"), TEXT("Scalability"), ECVF_SetByScalability, false);
    UE::ConfigUtilities::ApplyCVarSettingsFromIni(
      TEXT("GlobalIlluminationQuality@0"), TEXT("Scalability"), ECVF_SetByScalability, false);
    UE::ConfigUtilities::ApplyCVarSettingsFromIni(
      TEXT("ReflectionQuality@0"), TEXT("Scalability"), ECVF_SetByScalability, false);
    UE::ConfigUtilities::ApplyCVarSettingsFromIni(
      TEXT("PostProcessQuality@0"), TEXT("Scalability"), ECVF_SetByScalability, false);
    UE::ConfigUtilities::ApplyCVarSettingsFromIni(
      TEXT("TextureQuality@0"), TEXT("Scalability"), ECVF_SetByScalability, false);
    UE::ConfigUtilities::ApplyCVarSettingsFromIni(
      TEXT("EffectsQuality@0"), TEXT("Scalability"), ECVF_SetByScalability, false);
    UE::ConfigUtilities::ApplyCVarSettingsFromIni(
      TEXT("FoliageQuality@0"), TEXT("Scalability"), ECVF_SetByScalability, false);
    UE::ConfigUtilities::ApplyCVarSettingsFromIni(
      TEXT("ShadingQuality@0"), TEXT("Scalability"), ECVF_SetByScalability, false);
    break;
  }
}
