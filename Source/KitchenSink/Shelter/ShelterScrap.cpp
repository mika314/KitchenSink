#include "ShelterScrap.h"
#include "ShelterCharacter.h"

AShelterScrap::AShelterScrap(): snd(CreateDefaultSubobject<UAudioComponent>("snd"))
{
  auto mesh = GetStaticMeshComponent();
  mesh->SetStaticMesh(OBJ_FINDER(StaticMesh, "1-Shelter", "SM_ShelterScrap"));
  mesh->SetSimulatePhysics(true);
  mesh->SetCollisionProfileName(FName("PhysicsActor"));
  mesh->SetMobility(EComponentMobility::Movable);
  mesh->OnComponentHit.AddDynamic(this, &AShelterScrap::onHit);
  snd->SetupAttachment(mesh);
  snd->SetSound(OBJ_FINDER(SoundCue, "1-Shelter/Snd", "SND_Scrap01_Cue"));
}

auto AShelterScrap::onHit(UPrimitiveComponent *,
                          AActor *otherActor,
                          UPrimitiveComponent *,
                          FVector,
                          const FHitResult &) -> void
{
  if (auto character = Cast<AShelterCharacter>(otherActor))
  {
    LOG("character");
    character->addScrap();
    Destroy();
  }
}
