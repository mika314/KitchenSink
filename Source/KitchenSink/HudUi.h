// (c) 2023 Mika Pi

#pragma once

#include <Blueprint/UserWidget.h>
#include <CoreMinimal.h>

#include "HudUi.generated.h"

UCLASS()
class KITCHENSINK_API UHudUi : public UUserWidget
{
  GENERATED_BODY()
public:
  auto toggleSettings() -> void;
};
