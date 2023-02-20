// (c) 2023 Mika Pi

#include "SettingsUi.h"
#include "Settings.h"
#include <Components/Slider.h>
#include <Misc/ConfigUtilities.h>

auto USettingsUi::OnWidgetRebuilt() -> void
{
  Super::OnWidgetRebuilt();
  auto settings = Cast<USettings>(UGameplayStatics::LoadGameFromSlot("settings", 0));
  if (!settings)
    settings = NewObject<USettings>(this, USettings::StaticClass());

  const auto graphicsQuality = static_cast<GraphicsQuality>(settings->graphicsQuality);

  setGraphicsQuality(graphicsQuality, false);

  {
    auto widget = getProp<UCheckBox>(this, TEXT("GraphicsQualityLow"));
    CHECK_RET(widget);
    widget->SetCheckedState(graphicsQuality == GraphicsQuality::low ? ECheckBoxState::Checked
                                                                    : ECheckBoxState::Unchecked);
    widget->OnCheckStateChanged.AddDynamic(this, &USettingsUi::onGraphicsQualityLowChanged);
  }
  {
    auto widget = getProp<UCheckBox>(this, TEXT("GraphicsQualityMed"));
    CHECK_RET(widget);
    widget->SetCheckedState(graphicsQuality == GraphicsQuality::med ? ECheckBoxState::Checked
                                                                    : ECheckBoxState::Unchecked);
    widget->OnCheckStateChanged.AddDynamic(this, &USettingsUi::onGraphicsQualityMedChanged);
  }
  {
    auto widget = getProp<UCheckBox>(this, TEXT("GraphicsQualityHigh"));
    CHECK_RET(widget);
    widget->SetCheckedState(graphicsQuality == GraphicsQuality::high ? ECheckBoxState::Checked
                                                                     : ECheckBoxState::Unchecked);
    widget->OnCheckStateChanged.AddDynamic(this, &USettingsUi::onGraphicsQualityHighChanged);
  }
  {
    auto widget = getProp<UCheckBox>(this, TEXT("GraphicsQualityEpic"));
    CHECK_RET(widget);
    widget->SetCheckedState(graphicsQuality == GraphicsQuality::epic ? ECheckBoxState::Checked
                                                                     : ECheckBoxState::Unchecked);
    widget->OnCheckStateChanged.AddDynamic(this, &USettingsUi::onGraphicsQualityEpicChanged);
  }
  {
    auto widget = getProp<USlider>(this, TEXT("MouseSensitivitySlider"));
    CHECK_RET(widget);
    widget->OnValueChanged.AddDynamic(this, &USettingsUi::onMouseSensitivityChanged);
    widget->SetValue(settings->mouseSensitivity);
  }
  {
    auto widget = getProp<UTextBlock>(this, TEXT("MouseSensitivityTb"));
    CHECK_RET(widget);
    widget->SetText(FText::Format(LOC("{0}"), settings->mouseSensitivity));
  }
}

auto USettingsUi::onGraphicsQualityLowChanged(bool) -> void
{
  {
    auto widget = getProp<UCheckBox>(this, TEXT("GraphicsQualityLow"));
    CHECK_RET(widget);
    widget->SetCheckedState(ECheckBoxState::Checked);
  }
  {
    auto widget = getProp<UCheckBox>(this, TEXT("GraphicsQualityMed"));
    CHECK_RET(widget);
    widget->SetCheckedState(ECheckBoxState::Unchecked);
  }
  {
    auto widget = getProp<UCheckBox>(this, TEXT("GraphicsQualityHigh"));
    CHECK_RET(widget);
    widget->SetCheckedState(ECheckBoxState::Unchecked);
  }
  {
    auto widget = getProp<UCheckBox>(this, TEXT("GraphicsQualityEpic"));
    CHECK_RET(widget);
    widget->SetCheckedState(ECheckBoxState::Unchecked);
  }
  setGraphicsQuality(GraphicsQuality::low);
}

auto USettingsUi::onGraphicsQualityMedChanged(bool) -> void
{
  {
    auto widget = getProp<UCheckBox>(this, TEXT("GraphicsQualityLow"));
    CHECK_RET(widget);
    widget->SetCheckedState(ECheckBoxState::Unchecked);
  }
  {
    auto widget = getProp<UCheckBox>(this, TEXT("GraphicsQualityMed"));
    CHECK_RET(widget);
    widget->SetCheckedState(ECheckBoxState::Checked);
  }
  {
    auto widget = getProp<UCheckBox>(this, TEXT("GraphicsQualityHigh"));
    CHECK_RET(widget);
    widget->SetCheckedState(ECheckBoxState::Unchecked);
  }
  {
    auto widget = getProp<UCheckBox>(this, TEXT("GraphicsQualityEpic"));
    CHECK_RET(widget);
    widget->SetCheckedState(ECheckBoxState::Unchecked);
  }
  setGraphicsQuality(GraphicsQuality::med);
}

auto USettingsUi::onGraphicsQualityHighChanged(bool) -> void
{
  {
    auto widget = getProp<UCheckBox>(this, TEXT("GraphicsQualityLow"));
    CHECK_RET(widget);
    widget->SetCheckedState(ECheckBoxState::Unchecked);
  }
  {
    auto widget = getProp<UCheckBox>(this, TEXT("GraphicsQualityMed"));
    CHECK_RET(widget);
    widget->SetCheckedState(ECheckBoxState::Unchecked);
  }
  {
    auto widget = getProp<UCheckBox>(this, TEXT("GraphicsQualityHigh"));
    CHECK_RET(widget);
    widget->SetCheckedState(ECheckBoxState::Checked);
  }
  {
    auto widget = getProp<UCheckBox>(this, TEXT("GraphicsQualityEpic"));
    CHECK_RET(widget);
    widget->SetCheckedState(ECheckBoxState::Unchecked);
  }
  setGraphicsQuality(GraphicsQuality::high);
}

auto USettingsUi::onGraphicsQualityEpicChanged(bool) -> void
{
  {
    auto widget = getProp<UCheckBox>(this, TEXT("GraphicsQualityLow"));
    CHECK_RET(widget);
    widget->SetCheckedState(ECheckBoxState::Unchecked);
  }
  {
    auto widget = getProp<UCheckBox>(this, TEXT("GraphicsQualityMed"));
    CHECK_RET(widget);
    widget->SetCheckedState(ECheckBoxState::Unchecked);
  }
  {
    auto widget = getProp<UCheckBox>(this, TEXT("GraphicsQualityHigh"));
    CHECK_RET(widget);
    widget->SetCheckedState(ECheckBoxState::Unchecked);
  }
  {
    auto widget = getProp<UCheckBox>(this, TEXT("GraphicsQualityEpic"));
    CHECK_RET(widget);
    widget->SetCheckedState(ECheckBoxState::Checked);
  }
  setGraphicsQuality(GraphicsQuality::epic);
}

auto USettingsUi::setGraphicsQuality(GraphicsQuality v, bool save) -> void
{
  if (save)
  {
    auto settings = Cast<USettings>(UGameplayStatics::LoadGameFromSlot("settings", 0));
    if (!settings)
      settings = NewObject<USettings>(this, USettings::StaticClass());
    settings->graphicsQuality = static_cast<int>(v);
    UGameplayStatics::SaveGameToSlot(settings, "settings", 0);
  }

  auto controller = UGameplayStatics::GetPlayerController(this, 0);
  switch (v)
  {
  case GraphicsQuality::epic:
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
  case GraphicsQuality::high:
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
  case GraphicsQuality::med:
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
  case GraphicsQuality::low:
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

auto USettingsUi::onMouseSensitivityChanged(float v) -> void
{
  auto settings = Cast<USettings>(UGameplayStatics::LoadGameFromSlot("settings", 0));
  if (!settings)
    settings = NewObject<USettings>(this, USettings::StaticClass());
  settings->mouseSensitivity = v;
  UGameplayStatics::SaveGameToSlot(settings, "settings", 0);
  {
    auto widget = getProp<UTextBlock>(this, TEXT("MouseSensitivityTb"));
    CHECK_RET(widget);
    widget->SetText(FText::Format(LOC("{0}"), settings->mouseSensitivity));
  }
}
