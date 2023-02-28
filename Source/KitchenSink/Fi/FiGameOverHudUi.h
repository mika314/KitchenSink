// (c) 2023 Mika Pi

#pragma once

#include <Blueprint/UserWidget.h>
#include <CoreMinimal.h>

#include "FiGameOverHudUi.generated.h"

UCLASS()
class KITCHENSINK_API UFiGameOverHudUi final : public UUserWidget
{
  GENERATED_BODY()
public:
private:
  auto OnWidgetRebuilt() -> void final;
  UFUNCTION()
  void onTryAgainClicked();
  UFUNCTION()
  void onLobbyClicked();
};
