// (c) 2023 Mika Pi

#pragma once

#include <CoreMinimal.h>
#include <Engine/StaticMeshActor.h>

#include "FiIsland.generated.h"

UCLASS()
class KITCHENSINK_API AFiIsland final : public AStaticMeshActor
{
  GENERATED_BODY()
public:
  AFiIsland();

private:
  auto BeginPlay() -> void final;
  auto Tick(float) -> void final;

  FVector initLoc;
};
