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
  void onHit(UPrimitiveComponent *hitComp,
             AActor *otherActor,
             UPrimitiveComponent *otherComp,
             FVector normalImpulse,
             const FHitResult &hit);
};
