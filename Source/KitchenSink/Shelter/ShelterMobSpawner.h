#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "ShelterMobSpawner.generated.h"

UCLASS()
class AShelterMobSpawner final : public AActor
{
  GENERATED_BODY()

public:
  AShelterMobSpawner();

private:
  auto BeginPlay() -> void final;
  auto EndPlay(const EEndPlayReason::Type) -> void final;

  int32 MobsSpawned = 0;
  float TimeSinceLastSpawn = 0.0f;
  FTimerHandle SpawnTimerHandle;

  UPROPERTY(VisibleAnywhere)
  class USceneComponent *Root;

  UPROPERTY(EditAnywhere, Category = "Spawning")
  float SpawnRadius = 500.0f;

  UPROPERTY(EditAnywhere, Category = "Spawning")
  TSubclassOf<class AShelterMob> MobToSpawn;

  UPROPERTY(EditAnywhere, Category = "Spawning")
  int SpawnAmount = 5;

  UPROPERTY(EditAnywhere, Category = "Spawning")
  float SpawnDelay = 3.0f;

  auto SpawnMobs() -> void;
};
