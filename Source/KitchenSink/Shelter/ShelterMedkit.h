#pragma once

#include <Engine/StaticMeshActor.h>

#include "ShelterMedkit.generated.h"

UCLASS()
class AShelterMedkit final : public AStaticMeshActor
{
  GENERATED_BODY()
public:
  AShelterMedkit();

private:
  UFUNCTION()
  void onHit(UPrimitiveComponent *hitComp,
             AActor *otherActor,
             UPrimitiveComponent *otherComp,
             FVector normalImpulse,
             const FHitResult &hit);

  UAudioComponent *snd;
};
