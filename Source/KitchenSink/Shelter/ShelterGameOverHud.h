// (c) 2023 Mika Pi

#pragma once

#include <CoreMinimal.h>
#include <GameFramework/HUD.h>

#include "ShelterGameOverHud.generated.h"

UCLASS()
class KITCHENSINK_API AShelterGameOverHud : public AHUD
{
  GENERATED_BODY()
public:
  AShelterGameOverHud();

  auto getHudUi() -> class UShelterGameOverHudUi * { return hudUi; }

private:
  auto BeginPlay() -> void final;
  class UShelterGameOverHudUi *hudUi = nullptr;
};
