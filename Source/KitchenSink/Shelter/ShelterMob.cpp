#include "ShelterMob.h"
#include "ShelterCharacter.h"
#include "ShelterDestroyPoint.h"
#include "ShelterMedkit.h"
#include "ShelterScrap.h"
#include "ShelterShelter.h"
#include <Animation/AnimBlueprint.h>
#include <Animation/AnimMontage.h>
#include <Kismet/GameplayStatics.h>

AShelterMob::AShelterMob()
  : attackMontage(OBJ_FINDER(AnimMontage, "Quaternius/Animations", "Punch_Montage")),
    deathMontage(OBJ_FINDER(AnimMontage, "Quaternius/Animations", "Death_Montage")),
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

  auto moveComp =
    Cast<UCharacterMovementComponent>(GetComponentByClass(UCharacterMovementComponent::StaticClass()));
  CHECK_RET(moveComp);
  moveComp->MaxWalkSpeed = 300.0f;

  auto animInst = Cast<UAnimInstance>(GetMesh()->GetAnimInstance());
  CHECK_RET(animInst);
  animInst->OnMontageEnded.AddDynamic(this, &AShelterMob::onMontageEnded);
  animInst->OnMontageBlendingOut.AddDynamic(this, &AShelterMob::onMontageBlendingOut);

  setupAi();
}

auto AShelterMob::setupAi() -> void
{
  auto aiController = Cast<AAIController>(GetController());
  CHECK_RET(aiController);
  aiController->ReceiveMoveCompleted.AddDynamic(this, &AShelterMob::onMoveToActorFinished);
  state = EShelterMobState::processing;
}

auto AShelterMob::onMoveToActorFinished(FAIRequestID, EPathFollowingResult::Type) -> void
{
  processState();
}

auto AShelterMob::processState() -> void
{
  if (state == EShelterMobState::dead)
    return;
  auto character = Cast<AShelterCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
  CHECK_RET(character);

  const auto distToPlayer = (character->GetActorLocation() - GetActorLocation()).Size();
  if (distToPlayer < 350.f)
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
    auto animInst = Cast<UAnimInstance>(GetMesh()->GetAnimInstance());
    CHECK_RET(animInst);
    animInst->Montage_Play(attackMontage, 1.0f);
    state = EShelterMobState::busy;
    break;
  }
  case EShelterMobState::processing: {
    auto aiController = Cast<AAIController>(GetController());
    CHECK_RET(aiController);
    auto target = [&]() -> AActor * {
      if (rand() % 2 == 0)
        return GetWorld()->GetFirstPlayerController()->GetPawn();
      else
      {
        TArray<AActor *> shelterDestroyPoints;

        UGameplayStatics::GetAllActorsOfClass(
          GetWorld(), AShelterDestroyPoint::StaticClass(), shelterDestroyPoints);

        AActor *closestShelterDestroyPoint = nullptr;
        auto closestDistance = MAX_FLT;
        for (auto shelterDestroyPoint : shelterDestroyPoints)
        {
          const auto distance = GetDistanceTo(shelterDestroyPoint);
          if (distance < closestDistance)
          {
            closestShelterDestroyPoint = shelterDestroyPoint;
            closestDistance = distance;
          }
        }
        return closestShelterDestroyPoint;
      }
    }();
    CHECK_RET(target);
    auto ret = aiController->MoveToActor(target, 100.0f, true, true, true, 0, true);
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

void AShelterMob::onMontageEnded(UAnimMontage *anim, bool)
{
  if (state == EShelterMobState::dead)
    return;
  if (anim == attackMontage)
    lineTraceToDetermineHit();
  else
    processState();
}

auto AShelterMob::lineTraceToDetermineHit() -> void
{

  auto character = Cast<AShelterCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
  CHECK_RET(character);

  const auto distToPlayer = (character->GetActorLocation() - GetActorLocation()).Size();
  if (distToPlayer < 350.f)
  {
    LOG("Hit player by distance");
    character->applyDamage(0.03f);
    state = EShelterMobState::attacking;
    return;
  }

  const auto start = GetActorLocation();
  const auto end = start + GetActorForwardVector() * 500.f;
  FHitResult hitResult;
  bool bHit = GetWorld()->LineTraceSingleByChannel(hitResult, start, end, ECC_Visibility);
  if (!bHit)
  {
    LOG("Did not hit anything");
    state = EShelterMobState::processing;
    return;
  }
  AActor *hitActor = hitResult.GetActor();

  if (hitActor->IsA<AShelterCharacter>())
  {
    LOG("Hit player");
    character->applyDamage(0.03f);
    state = EShelterMobState::attacking;
    return;
  }
  if (hitActor->IsA<AShelterShelter>())
  {
    LOG("Hit shelter building");
    character->applyShelterDamage(0.03f);
    state = EShelterMobState::attacking;
    return;
  }

  LOG("Hit something else", hitActor->GetName());
  state = EShelterMobState::processing;
}

void AShelterMob::onMontageBlendingOut(UAnimMontage *anim, bool)
{
  if (state == EShelterMobState::dead && anim == deathMontage)
  {
    Destroy();

    if (rand() % 10 == 0)
    {
      auto newActor = [&]() -> AActor * {
        const auto spawnLoc = getLoc(this) + vec(0., 0., 150.f);
        FActorSpawnParameters ActorSpawnParams;
        ActorSpawnParams.SpawnCollisionHandlingOverride =
          ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

        auto World = GetWorld();
        CHECK_RET(World, nullptr);
        if (rand() % 2 == 0)
          return World->SpawnActor<AActor>(
            AShelterScrap::StaticClass(), spawnLoc, rot(0., 0., 0.), ActorSpawnParams);
        else
          return World->SpawnActor<AActor>(
            AShelterMedkit::StaticClass(), spawnLoc, rot(0., 0., 0.), ActorSpawnParams);
      }();
      if (newActor)
      {
        auto primitiveComp = Cast<UPrimitiveComponent>(newActor->GetRootComponent());
        CHECK_RET(primitiveComp);
        primitiveComp->AddImpulse(
          vec(FMath::RandRange(-200.f, 200.f), FMath::RandRange(-200.f, 200.f), 600.f), NAME_None, true);
      }
    }
  }
}

auto AShelterMob::die() -> void
{
  if (state == EShelterMobState::dead)
    return;
  LOG("Death Montage", GetWorld()->GetTimeSeconds());
  auto animInst = Cast<UAnimInstance>(GetMesh()->GetAnimInstance());
  CHECK_RET(animInst);
  animInst->Montage_Play(deathMontage, 1.0f);
  state = EShelterMobState::dead;
  SetActorEnableCollision(false);
}

auto AShelterMob::EndPlay(const EEndPlayReason::Type reason) -> void
{
  auto animInst = Cast<UAnimInstance>(GetMesh()->GetAnimInstance());
  CHECK_RET(animInst);
  animInst->OnMontageEnded.RemoveDynamic(this, &AShelterMob::onMontageEnded);
  animInst->OnMontageBlendingOut.RemoveDynamic(this, &AShelterMob::onMontageBlendingOut);
  Super::EndPlay(reason);
}
