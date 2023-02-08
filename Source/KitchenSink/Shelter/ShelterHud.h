// (c) 2023 Mika Pi

#pragma once

#include <CoreMinimal.h>
#include <GameFramework/HUD.h>

#include "ShelterHud.generated.h"

UCLASS()
class KITCHENSINK_API AShelterHud final : public AHUD
{
  GENERATED_BODY()
public:
  AShelterHud();

  auto getHudUi() -> class UShelterHudUi * { return hudUi; }

private:
  auto BeginPlay() -> void final;

  UShelterHudUi *hudUi = nullptr;
};
