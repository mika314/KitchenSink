// (c) 2023 Mika Pi

#pragma once

#include <Blueprint/UserWidget.h>
#include <CoreMinimal.h>

#include "SettingsUi.generated.h"

enum class VideoQuality { low, med, high, epic };

UCLASS()
class KITCHENSINK_API USettingsUi : public UUserWidget
{
  GENERATED_BODY()
private:
  auto OnWidgetRebuilt() -> void final;

  UFUNCTION()
  void onVideoQualityLowChanged(bool isChecked);
  UFUNCTION()
  void onVideoQualityMedChanged(bool isChecked);
  UFUNCTION()
  void onVideoQualityHighChanged(bool isChecked);
  UFUNCTION()
  void onVideoQualityEpicChanged(bool isChecked);

  auto setVideoQuality(VideoQuality, bool save = true) -> void;
};
