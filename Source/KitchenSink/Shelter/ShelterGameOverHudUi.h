// (c) 2023 Mika Pi

#pragma once

#include <Blueprint/UserWidget.h>
#include <CoreMinimal.h>

#include "ShelterGameOverHudUi.generated.h"

UCLASS()
class KITCHENSINK_API UShelterGameOverHudUi : public UUserWidget
{
  GENERATED_BODY()

private:
  auto OnWidgetRebuilt() -> void final;
  UFUNCTION()
  void onTryAgainClicked();
  UFUNCTION()
  void onLobbyClicked();
};
