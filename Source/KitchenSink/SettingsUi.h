// (c) 2023 Mika Pi

#pragma once

#include <Blueprint/UserWidget.h>
#include <CoreMinimal.h>

#include "SettingsUi.generated.h"

enum class GraphicsQuality { low, med, high, epic };

UCLASS()
class KITCHENSINK_API USettingsUi : public UUserWidget
{
  GENERATED_BODY()
private:
  auto OnWidgetRebuilt() -> void final;

  UFUNCTION()
  void onGraphicsQualityLowChanged(bool isChecked);
  UFUNCTION()
  void onGraphicsQualityMedChanged(bool isChecked);
  UFUNCTION()
  void onGraphicsQualityHighChanged(bool isChecked);
  UFUNCTION()
  void onGraphicsQualityEpicChanged(bool isChecked);
  UFUNCTION()
  void onMouseSensitivityChanged(float v);

  auto setGraphicsQuality(GraphicsQuality, bool save = true) -> void;
};
