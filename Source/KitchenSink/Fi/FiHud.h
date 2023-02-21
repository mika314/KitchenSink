// (c) 2023 Mika Pi

#pragma once

#include <CoreMinimal.h>
#include <GameFramework/HUD.h>

#include "FiHud.generated.h"

UCLASS()
class KITCHENSINK_API AFiHud final : public AHUD
{
  GENERATED_BODY()
public:
  AFiHud();

  auto getHudUi() -> class UFiHudUi * { return hudUi; }

private:
  auto BeginPlay() -> void final;

  UFiHudUi *hudUi = nullptr;
};
