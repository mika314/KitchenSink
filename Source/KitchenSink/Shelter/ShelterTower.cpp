#include "ShelterTower.h"
#include "RepairAmount.h"
#include "ShelterProjectile.h"

AShelterTower::AShelterTower()
  : projectileClass(CLASS_FINDER(AShelterProjectile, "1-Shelter", "BP_ShelterProjectile")),
    fireSound(OBJ_FINDER(SoundCue, "1-Shelter", "SND_Fire_Cue"))
{
  auto mesh = GetStaticMeshComponent();
  mesh->SetStaticMesh(OBJ_FINDER(StaticMesh, "1-Shelter", "SM_ShelterTower"));
  mesh->SetScalarParameterValueOnMaterials("HP", hp);
}

auto AShelterTower::BeginPlay() -> void
{
  Super::BeginPlay();
  GetWorld()->GetTimerManager().SetTimer(
    hFireTimer, [this]() { fire(); }, 5.f, true);
}

auto AShelterTower::EndPlay(const EEndPlayReason::Type reason) -> void
{
  GetWorld()->GetTimerManager().ClearTimer(hFireTimer);
  Super::EndPlay(reason);
}

auto AShelterTower::repair() -> bool
{
  if (hp >= 1.f)
    return false;
  hp += repairAmount();
  auto mesh = GetStaticMeshComponent();
  mesh->SetScalarParameterValueOnMaterials("HP", hp);
  return true;
}

auto AShelterTower::applyDamage() -> void
{
  hp -= 0.03f;
  if (hp <= 0)
    Destroy();
}

auto AShelterTower::fire() -> void
{
  auto mesh = GetStaticMeshComponent();
  auto world = GetWorld();
  CHECK_RET(world)

  for (auto i = 0; i < 20; ++i)
  {
    const auto r = rot(-30.f, 1. * (rand() % 360), 0.);
    const auto l = mesh->GetSocketLocation(FName("GunLoc")) + r.Vector() * 10'00.f;
    FActorSpawnParameters ActorSpawnParams;
    ActorSpawnParams.SpawnCollisionHandlingOverride =
      ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
    world->SpawnActor<AShelterProjectile>(projectileClass, l, r, ActorSpawnParams);
  }

  CHECK_RET(fireSound);
  UGameplayStatics::PlaySoundAtLocation(this, fireSound, getLoc(this));
}
