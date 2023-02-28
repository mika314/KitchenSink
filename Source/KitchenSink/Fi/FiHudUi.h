// (c) 2023 Mika Pi

#pragma once

#include <Blueprint/UserWidget.h>
#include <CoreMinimal.h>

#include "FiHudUi.generated.h"

UCLASS()
class KITCHENSINK_API UFiHudUi : public UUserWidget
{
  GENERATED_BODY()
public:
  auto showStars(int) -> void;
  auto updateHelp(FText) -> void;
  auto updateObjective(FText) -> void;
  auto updateShift(float) -> void;
};
