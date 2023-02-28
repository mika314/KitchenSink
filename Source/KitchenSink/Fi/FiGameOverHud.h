// (c) 2023 Mika Pi

#pragma once

#include <CoreMinimal.h>
#include <GameFramework/HUD.h>

#include "FiGameOverHud.generated.h"

UCLASS()
class KITCHENSINK_API AFiGameOverHud final : public AHUD
{
  GENERATED_BODY()
public:
  AFiGameOverHud();
  auto getHudUi() -> class UFiGameOverHudUi * { return hudUi; }

private:
  auto BeginPlay() -> void final;

  class UFiGameOverHudUi *hudUi;
};
