#include "ShelterMob.h"
#include "Kismet/GameplayStatics.h"
#include <Animation/AnimBlueprint.h>
#include <Animation/AnimMontage.h>

AShelterMob::AShelterMob()
  : AttackMontage(OBJ_FINDER(AnimMontage, "Quaternius/Animations", "Punch_Montage")),
    DeathMontage(OBJ_FINDER(AnimMontage, "Quaternius/Animations", "Death_Montage"))
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

  isAlive = true;

  auto MoveComp =
    Cast<UCharacterMovementComponent>(GetComponentByClass(UCharacterMovementComponent::StaticClass()));
  CHECK_RET(MoveComp);

  MoveComp->MaxWalkSpeed = 300.0f;

  auto AIController = Cast<AAIController>(GetController());
  CHECK_RET(AIController);

  auto AnimInstance = Cast<UAnimInstance>(GetMesh()->GetAnimInstance());
  CHECK_RET(AnimInstance);
  AnimInstance->OnMontageEnded.AddDynamic(this, &AShelterMob::OnMontageEnded);
  AnimInstance->OnMontageBlendingOut.AddDynamic(this, &AShelterMob::OnMontageBlendingOut);

  AIController->ReceiveMoveCompleted.AddDynamic(this, &AShelterMob::OnMoveToActorFinished);

  APawn *PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
  CHECK_RET(PlayerPawn);

  auto ret = AIController->MoveToActor(PlayerPawn, 300.0f, true, true, true, 0, true);
  switch (ret)
  {
  case EPathFollowingRequestResult::Failed: state = EShelterMobState::patrolling; break;
  case EPathFollowingRequestResult::AlreadyAtGoal: state = EShelterMobState::attacking; break;
  case EPathFollowingRequestResult::RequestSuccessful: state = EShelterMobState::busy; break;
  }
}

auto AShelterMob::OnMoveToActorFinished(FAIRequestID, EPathFollowingResult::Type) -> void
{
  LOG("OnMoveToActorFinished");
  processState();
}

auto AShelterMob::processState() -> void
{

  if (state == EShelterMobState::dead)
  {
    Destroy();
    return;
  }

  APawn *PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
  CHECK_RET(PlayerPawn);
  const auto DistanceToPlayer = (PlayerPawn->GetActorLocation() - GetActorLocation()).Size();

  if (DistanceToPlayer < 350.f)
  {
    LOG("attack", DistanceToPlayer);
    state = EShelterMobState::attacking;
  }
  else if (DistanceToPlayer < 1500.f)
  {
    LOG("processing", DistanceToPlayer);
    state = EShelterMobState::processing;
  }
  else
  {
    LOG("patrolling", DistanceToPlayer);
    state = EShelterMobState::patrolling;
  }
}

EShelterMobState AShelterMob::getState() const
{
  return state;
}

auto AShelterMob::Tick(float dt) -> void
{
  Super::Tick(dt);
  APawn *PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
  CHECK_RET(PlayerPawn);
  switch (state)
  {
  case EShelterMobState::attacking: {
    LOG("Attack Montage", GetWorld()->GetTimeSeconds());
    auto AnimInstance = Cast<UAnimInstance>(GetMesh()->GetAnimInstance());
    CHECK_RET(AnimInstance);
    AnimInstance->Montage_Play(AttackMontage, 1.0f);
    state = EShelterMobState::busy;
    break;
  }
  case EShelterMobState::processing: {
    LOG("Processing");
    auto AIController = Cast<AAIController>(GetController());
    CHECK_RET(AIController);
    auto ret = AIController->MoveToActor(PlayerPawn, 100.0f, true, true, true, 0, true);
    switch (ret)
    {
    case EPathFollowingRequestResult::Failed: state = EShelterMobState::patrolling; break;
    case EPathFollowingRequestResult::AlreadyAtGoal: state = EShelterMobState::attacking; break;
    case EPathFollowingRequestResult::RequestSuccessful: state = EShelterMobState::busy; break;
    }
    break;
  }
  }
}

void AShelterMob::OnMontageEnded(UAnimMontage *, bool)
{
  LOG("Montage Ended", GetWorld()->GetTimeSeconds());
  processState();
}

void AShelterMob::OnMontageBlendingOut(UAnimMontage *anim, bool)
{
  LOG("Montage BledningOut", GetWorld()->GetTimeSeconds());

  if (state == EShelterMobState::dead)
    Destroy();
}

auto AShelterMob::die() -> void
{
  if (state == EShelterMobState::dead)
    return;
  LOG("Death Montage", GetWorld()->GetTimeSeconds());
  auto AnimInstance = Cast<UAnimInstance>(GetMesh()->GetAnimInstance());
  CHECK_RET(AnimInstance);
  AnimInstance->Montage_Play(DeathMontage, 1.0f);
  state = EShelterMobState::dead;
}
