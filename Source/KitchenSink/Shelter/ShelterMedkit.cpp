#include "ShelterMedkit.h"
#include "ShelterCharacter.h"

AShelterMedkit::AShelterMedkit() : snd(CreateDefaultSubobject<UAudioComponent>("snd"))
{
  auto mesh = GetStaticMeshComponent();
  mesh->SetStaticMesh(OBJ_FINDER(StaticMesh, "1-Shelter", "SM_ShelterMedkit"));
  mesh->SetSimulatePhysics(true);
  mesh->SetCollisionProfileName(FName("PhysicsActor"));
  mesh->SetMobility(EComponentMobility::Movable);
  mesh->OnComponentHit.AddDynamic(this, &AShelterMedkit::onHit);
  snd->AttachToComponent(mesh, FAttachmentTransformRules::SnapToTargetIncludingScale);
  snd->SetSound(OBJ_FINDER(SoundCue, "1-Shelter/Snd", "SND_Medkit_Cue"));
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
