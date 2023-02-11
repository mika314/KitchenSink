#include "ShelterMob.h"
#include "ShelterCharacter.h"
#include "ShelterDestroyPoint.h"
#include "ShelterScrap.h"
#include "ShelterShelter.h"
#include <Animation/AnimBlueprint.h>
#include <Animation/AnimMontage.h>
#include <Kismet/GameplayStatics.h>

AShelterMob::AShelterMob()
  : AttackMontage(OBJ_FINDER(AnimMontage, "Quaternius/Animations", "Punch_Montage")),
    DeathMontage(OBJ_FINDER(AnimMontage, "Quaternius/Animations", "Death_Montage")),
    mushroomMesh(CreateDefaultSubobject<UStaticMeshComponent>("mushrumMesh"))
{
  auto mesh = GetMesh();
  mesh->SetSkeletalMesh(OBJ_FINDER(SkeletalMesh, "Quaternius/Mesh", "SK_MushroomKing"));
  mesh->SetRelativeScale3D(vec(0.005f, 0.005f, 0.005f));
  mesh->SetRelativeLocation(vec(0.f, 0.f, -86.5f));
  mesh->SetRelativeRotation(rot(0.f, -90.f, 0.f));
  mesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
  mesh->SetAnimInstanceClass(CLASS_FINDER(UAnimInstance, "Quaternius/Bluprints", "BP_MushroomKingAnim"));

  mushroomMesh->SetStaticMesh(OBJ_FINDER(StaticMesh, "1-Shelter", "SM_Mushroom"));
  mushroomMesh->AttachToComponent(
    mesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("LArm"));

  AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

auto AShelterMob::BeginPlay() -> void
{
  Super::BeginPlay();

  auto MoveComp =
    Cast<UCharacterMovementComponent>(GetComponentByClass(UCharacterMovementComponent::StaticClass()));
  CHECK_RET(MoveComp);
  MoveComp->MaxWalkSpeed = 300.0f;

  auto AnimInstance = Cast<UAnimInstance>(GetMesh()->GetAnimInstance());
  CHECK_RET(AnimInstance);
  AnimInstance->OnMontageEnded.AddDynamic(this, &AShelterMob::OnMontageEnded);
  AnimInstance->OnMontageBlendingOut.AddDynamic(this, &AShelterMob::OnMontageBlendingOut);

  setupAi();
}

auto AShelterMob::setupAi() -> void
{
  auto AIController = Cast<AAIController>(GetController());
  CHECK_RET(AIController);

  AIController->ReceiveMoveCompleted.AddDynamic(this, &AShelterMob::OnMoveToActorFinished);

  state = EShelterMobState::processing;
}

auto AShelterMob::OnMoveToActorFinished(FAIRequestID, EPathFollowingResult::Type) -> void
{
  processState();
}

auto AShelterMob::processState() -> void
{
  if (state == EShelterMobState::dead)
    return;
  auto character = Cast<AShelterCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
  CHECK_RET(character);

  const auto DistanceToPlayer = (character->GetActorLocation() - GetActorLocation()).Size();

  if (DistanceToPlayer < 350.f)
    state = EShelterMobState::attacking;
  else
    state = EShelterMobState::processing;
}

EShelterMobState AShelterMob::getState() const
{
  return state;
}

auto AShelterMob::Tick(float dt) -> void
{
  Super::Tick(dt);
  if (state == EShelterMobState::dead)
    return;
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
    auto AIController = Cast<AAIController>(GetController());
    CHECK_RET(AIController);
    auto target = [&]() -> AActor * {
      if (rand() % 2 == 0)
        return GetWorld()->GetFirstPlayerController()->GetPawn();
      else
      {
        TArray<AActor *> ShelterDestroyPoints;

        UGameplayStatics::GetAllActorsOfClass(
          GetWorld(), AShelterDestroyPoint::StaticClass(), ShelterDestroyPoints);

        AActor *ClosestShelterDestroyPoint = nullptr;
        auto ClosestDistance = MAX_FLT;
        for (auto ShelterDestroyPoint : ShelterDestroyPoints)
        {
          const auto Distance = GetDistanceTo(ShelterDestroyPoint);
          if (Distance < ClosestDistance)
          {
            ClosestShelterDestroyPoint = ShelterDestroyPoint;
            ClosestDistance = Distance;
          }
        }
        return ClosestShelterDestroyPoint;
      }
    }();
    CHECK_RET(target);
    auto ret = AIController->MoveToActor(target, 100.0f, true, true, true, 0, true);
    switch (ret)
    {
    case EPathFollowingRequestResult::Failed: state = EShelterMobState::processing; break;
    case EPathFollowingRequestResult::AlreadyAtGoal: state = EShelterMobState::attacking; break;
    case EPathFollowingRequestResult::RequestSuccessful: state = EShelterMobState::busy; break;
    }
  }
  break;
  }
}

void AShelterMob::OnMontageEnded(UAnimMontage *anim, bool)
{
  if (state == EShelterMobState::dead)
    return;
  if (anim == AttackMontage)
    LineTraceToDetermineHit();
  else
    processState();
}

auto AShelterMob::LineTraceToDetermineHit() -> void
{
  FVector Start = GetActorLocation();
  FVector End = Start + GetActorForwardVector() * 500.f;

  auto character = Cast<AShelterCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
  CHECK_RET(character);

  const auto DistanceToPlayer = (character->GetActorLocation() - GetActorLocation()).Size();
  if (DistanceToPlayer < 350.f)
  {
    LOG("Hit player by distance");
    character->applyDamage(0.03f);
    state = EShelterMobState::attacking;
    return;
  }

  FHitResult HitResult;
  bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility);
  if (!bHit)
  {
    LOG("Did not hit anything");
    state = EShelterMobState::processing;
    return;
  }
  AActor *HitActor = HitResult.GetActor();

  if (HitActor->IsA<AShelterCharacter>())
  {
    LOG("Hit player");
    character->applyDamage(0.03f);
    state = EShelterMobState::attacking;
    return;
  }
  if (HitActor->IsA<AShelterShelter>())
  {
    LOG("Hit shelter building");
    character->applyShelterDamage(0.03f);
    state = EShelterMobState::attacking;
    return;
  }

  LOG("Hit something else", HitActor->GetName());
  state = EShelterMobState::processing;
  return;
}

void AShelterMob::OnMontageBlendingOut(UAnimMontage *, bool)
{
  if (state == EShelterMobState::dead)
  {
    Destroy();

    if (rand() % 10 == 0)
    {
      const auto SpawnLocation = getLoc(this) + vec(0., 0., 150.f);
      FActorSpawnParameters ActorSpawnParams;
      ActorSpawnParams.SpawnCollisionHandlingOverride =
        ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

      auto World = GetWorld();
      CHECK_RET(World)
      auto NewActor = World->SpawnActor<AShelterScrap>(
        AShelterScrap::StaticClass(), SpawnLocation, rot(0., 0., 0.), ActorSpawnParams);
      if (NewActor)
      {
        auto PrimitiveComponent = Cast<UPrimitiveComponent>(NewActor->GetRootComponent());
        CHECK_RET(PrimitiveComponent);
        PrimitiveComponent->AddImpulse(
          FVector(FMath::RandRange(-200.f, 200.f), FMath::RandRange(-200.f, 200.f), 600.f),
          NAME_None,
          true);
      }
    }
  }
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

auto AShelterMob::EndPlay(const EEndPlayReason::Type reason) -> void
{
  auto AnimInstance = Cast<UAnimInstance>(GetMesh()->GetAnimInstance());
  CHECK_RET(AnimInstance);
  AnimInstance->OnMontageEnded.RemoveDynamic(this, &AShelterMob::OnMontageEnded);
  AnimInstance->OnMontageBlendingOut.RemoveDynamic(this, &AShelterMob::OnMontageBlendingOut);
  Super::EndPlay(reason);
}
