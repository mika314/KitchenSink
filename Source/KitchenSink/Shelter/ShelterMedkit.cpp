#include "ShelterMedkit.h"
#include "ShelterCharacter.h"

AShelterMedkit::AShelterMedkit()
{
  auto mesh = GetStaticMeshComponent();
  mesh->SetStaticMesh(OBJ_FINDER(StaticMesh, "1-Shelter", "SM_ShelterMedkit"));
  mesh->SetSimulatePhysics(true);
  mesh->SetCollisionProfileName(FName("PhysicsActor"));
  mesh->SetMobility(EComponentMobility::Movable);
  mesh->OnComponentHit.AddDynamic(this, &AShelterMedkit::onHit);
}

auto AShelterMedkit::onHit(UPrimitiveComponent *,
                           AActor *otherActor,
                           UPrimitiveComponent *,
                           FVector,
                           const FHitResult &) -> void
{
  if (auto character = Cast<AShelterCharacter>(otherActor))
  {
    LOG("character hit medkit");
    character->addMedkit();
    Destroy();
  }
}
