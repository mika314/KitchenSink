#include "ShelterScrap.h"
#include "ShelterCharacter.h"

AShelterScrap::AShelterScrap()
{
  auto mesh = GetStaticMeshComponent();
  mesh->SetStaticMesh(OBJ_FINDER(StaticMesh, "1-Shelter", "SM_ShelterScrap"));
  mesh->SetSimulatePhysics(true);
  mesh->SetCollisionProfileName(FName("PhysicsActor"));
  mesh->SetMobility(EComponentMobility::Movable);
  mesh->OnComponentHit.AddDynamic(this, &AShelterScrap::onHit);
}

auto AShelterScrap::BeginPlay() -> void
{
  Super::BeginPlay();
}

auto AShelterScrap::onHit(UPrimitiveComponent *,
                          AActor *OtherActor,
                          UPrimitiveComponent *,
                          FVector,
                          const FHitResult &) -> void
{
  if (auto character = Cast<AShelterCharacter>(OtherActor))
  {
    LOG("character");
    character->addScrap();
    Destroy();
  }
}
