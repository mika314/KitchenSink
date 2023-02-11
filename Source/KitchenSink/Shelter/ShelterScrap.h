#pragma once

#include <Engine/StaticMeshActor.h>

#include "ShelterScrap.generated.h"

UCLASS()
class AShelterScrap final : public AStaticMeshActor
{
  GENERATED_BODY()
public:
  AShelterScrap();

private:
  UFUNCTION()
  void onHit(UPrimitiveComponent *HitComp,
             AActor *OtherActor,
             UPrimitiveComponent *OtherComp,
             FVector NormalImpulse,
             const FHitResult &Hit);
};
