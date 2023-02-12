#pragma once

#include <Engine/StaticMeshActor.h>

#include "ShelterTower.generated.h"

UCLASS()
class AShelterTower final : public AStaticMeshActor
{
  GENERATED_BODY()
public:
  AShelterTower();
  auto repair() -> bool;
  auto applyDamage() -> void;

private:
  float hp = 0.025f;
  FTimerHandle hFireTimer;
  TSubclassOf<class AShelterProjectile> projectileClass;
  USoundBase *fireSound;

  auto BeginPlay() -> void final;
  auto EndPlay(const EEndPlayReason::Type) -> void final;
  auto fire() -> void;
};
