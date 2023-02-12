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
  auto spawnMobs() -> void;

  int mobsSpawned = 0;
  float timeSinceLastSpawn = 0.0f;
  FTimerHandle hSpawnTimer;

  UPROPERTY(VisibleAnywhere)
  class USceneComponent *Root;

  UPROPERTY(EditAnywhere, Category = "Spawning")
  float SpawnRadius = 7863.f;

  UPROPERTY(EditAnywhere, Category = "Spawning")
  TSubclassOf<class AShelterMob> MobToSpawn;

  UPROPERTY(EditAnywhere, Category = "Spawning")
  float SpawnDelay = 3.0f;
};
