// (c) 2023 Mika Pi

#pragma once

#include <Blueprint/UserWidget.h>
#include <CoreMinimal.h>

#include "ShelterHudUi.generated.h"

UCLASS()
class KITCHENSINK_API UShelterHudUi : public UUserWidget
{
  GENERATED_BODY()
public:
  auto setHp(float) -> void;
  auto setMedkits(int) -> void;
  auto setScrap(int) -> void;
  auto setShelterHp(float) -> void;
};
