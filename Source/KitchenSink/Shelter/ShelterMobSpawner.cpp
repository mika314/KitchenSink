#include "ShelterMobSpawner.h"
#include "ShelterMob.h"

AShelterMobSpawner::AShelterMobSpawner() : Root(CreateDefaultSubobject<USceneComponent>(TEXT("Root")))
{
  PrimaryActorTick.bCanEverTick = false;
  RootComponent = Root;
}

auto AShelterMobSpawner::BeginPlay() -> void
{
  Super::BeginPlay();

  GetWorld()->GetTimerManager().SetTimer(
    SpawnTimerHandle, [this]() { SpawnMobs(); }, SpawnDelay, true);
}

auto AShelterMobSpawner::SpawnMobs() -> void
{
  LOG("Spawn mobs", SpawnAmount);
  for (int i = 0; i < SpawnAmount; ++i)
  {
    ++MobsSpawned;

    const auto location = GetActorLocation();
    const auto angle = FMath::RandRange(0.0f, 360.0f);
    const auto x = location.X + SpawnRadius * FMath::Cos(angle);
    const auto y = location.Y + SpawnRadius * FMath::Sin(angle);
    const auto spawnLocation = vec(x, y, location.Z);
    GetWorld()->SpawnActor<AShelterMob>(MobToSpawn, spawnLocation, rot(0.0f, 0.0f, 0.0f));
  }
}

auto AShelterMobSpawner::EndPlay(const EEndPlayReason::Type reason) -> void
{
  GetWorld()->GetTimerManager().ClearTimer(SpawnTimerHandle);
  Super::EndPlay(reason);
}
