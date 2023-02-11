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
    hSpawnTimer, [this]() { spawnMobs(); }, SpawnDelay, true);
}

auto AShelterMobSpawner::spawnMobs() -> void
{
  LOG("Spawn mobs", SpawnAmount);
  for (auto i = 0; i < SpawnAmount; ++i)
  {
    ++mobsSpawned;
    const auto l = getLoc(this);
    const auto angle = FMath::RandRange(0.0f, 360.0f);
    const auto x = l.X + SpawnRadius * FMath::Cos(angle);
    const auto y = l.Y + SpawnRadius * FMath::Sin(angle);
    const auto spawnL = vec(x, y, l.Z);
    GetWorld()->SpawnActor<AShelterMob>(MobToSpawn, spawnL, rot(0.0f, 0.0f, 0.0f));
  }
}

auto AShelterMobSpawner::EndPlay(const EEndPlayReason::Type reason) -> void
{
  GetWorld()->GetTimerManager().ClearTimer(hSpawnTimer);
  Super::EndPlay(reason);
}
