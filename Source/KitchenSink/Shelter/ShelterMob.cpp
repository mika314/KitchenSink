#include "ShelterMob.h"
#include <Animation/AnimBlueprint.h>

AShelterMob::AShelterMob()
{
  auto mesh = GetMesh();
  mesh->SetSkeletalMesh(OBJ_FINDER(SkeletalMesh, "Quaternius/Mesh", "SK_MushroomKing"));
  mesh->SetRelativeScale3D(vec(0.005f, 0.005f, 0.005f));
  mesh->SetRelativeLocation(vec(0.f, 0.f, -86.5f));
  mesh->SetRelativeRotation(rot(0.f, -90.f, 0.f));
  mesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
  mesh->SetAnimInstanceClass(CLASS_FINDER(UAnimInstance, "Quaternius/Bluprints", "BP_MushroomKingAnim"));
}

auto AShelterMob::BeginPlay() -> void
{
  Super::BeginPlay();

  auto MoveComp =
    Cast<UCharacterMovementComponent>(GetComponentByClass(UCharacterMovementComponent::StaticClass()));
  CHECK_RET(MoveComp);

  MoveComp->MaxWalkSpeed = 300.0f;

  auto AIController = Cast<AAIController>(GetController());
  CHECK_RET(AIController);
  APawn *PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
  CHECK_RET(PlayerPawn);
  AIController->MoveToActor(PlayerPawn, 100.0f, true, true, true, 0, true);
}
