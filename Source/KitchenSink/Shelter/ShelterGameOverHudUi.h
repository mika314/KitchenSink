// (c) 2023 Mika Pi

#pragma once

#include <Blueprint/UserWidget.h>
#include <CoreMinimal.h>

#include "ShelterGameOverHudUi.generated.h"

UCLASS()
class KITCHENSINK_API UShelterGameOverHudUi : public UUserWidget
{
  GENERATED_BODY()
public:
  auto OnWidgetRebuilt() -> void final;

private:
  UFUNCTION()
  void onTryAgainClicked();
  UFUNCTION()
  void onLobbyClicked();
};
