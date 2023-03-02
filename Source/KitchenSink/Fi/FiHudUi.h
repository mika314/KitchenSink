// (c) 2023 Mika Pi

#pragma once

#include <Blueprint/UserWidget.h>
#include <CoreMinimal.h>

#include "FiHudUi.generated.h"

enum class Objective { pickup, delivery };

UCLASS()
class KITCHENSINK_API UFiHudUi : public UUserWidget
{
  GENERATED_BODY()
public:
  auto showStars(int) -> void;
  auto updateHelp(FText) -> void;
  auto updateIncome(int) -> void;
  auto update(Objective) -> void;
  auto updateShift(float) -> void;
};
