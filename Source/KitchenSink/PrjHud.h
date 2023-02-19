// (c) 2023 Mika Pi

#pragma once

#include <CoreMinimal.h>
#include <GameFramework/HUD.h>

#include "PrjHud.generated.h"

UCLASS()
class KITCHENSINK_API APrjHud final : public AHUD
{
  GENERATED_BODY()
public:
  APrjHud();
  auto getHudUi() -> class UHudUi * { return hudUi; }

private:
  auto BeginPlay() -> void final;

  UHudUi *hudUi = nullptr;
};
