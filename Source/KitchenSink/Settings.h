// (c) 2023 Mika Pi

#pragma once

#include <CoreMinimal.h>
#include <GameFramework/SaveGame.h>

#include "Settings.generated.h"

UCLASS()
class KITCHENSINK_API USettings : public USaveGame
{
  GENERATED_BODY()
public:
  UPROPERTY()
  int graphicsQuality = 3;

  UPROPERTY()
  float mouseSensitivity = 0.5f;
};
