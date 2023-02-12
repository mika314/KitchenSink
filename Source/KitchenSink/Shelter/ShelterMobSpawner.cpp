#include "ShelterMobSpawner.h"
#include "RepairAmount.h"
#include "ShelterCharacter.h"
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
  auto character = Cast<AShelterCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
  CHECK_RET(character);
  auto spawnAmountFloat =
    2.f * std::exp(GetWorld()->GetTimeSeconds() * 0.005f *
                   std::min(1.f, character->getShelterHp() + character->getScrap() * repairAmount()));
  float intPart;
  const auto fracPart = std::modf(spawnAmountFloat, &intPart);
  const auto spawnAmount =
    static_cast<int>(intPart + 0.01f) + (rand() % 1000 > static_cast<int>(fracPart * 1000) ? 0 : 1);

  LOG("Spawn mobs", spawnAmount);
  for (auto i = 0; i < spawnAmount; ++i)
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
