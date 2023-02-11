#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "ShelterMob.generated.h"

UENUM(BlueprintType)
enum class EShelterMobState : uint8 { busy, attacking, processing, dead };

UCLASS()
class AShelterMob final : public ACharacter
{
  GENERATED_BODY()
public:
  AShelterMob();

  UFUNCTION(BlueprintCallable)
  EShelterMobState getState() const;

  auto die() -> void;

private:
  auto BeginPlay() -> void final;
  auto EndPlay(const EEndPlayReason::Type) -> void final;
  auto Tick(float) -> void final;
  auto processState() -> void;
  auto setupAi() -> void;
  auto LineTraceToDetermineHit() -> void;

  UFUNCTION()
  void OnMoveToActorFinished(FAIRequestID reqId, EPathFollowingResult::Type Result);

  UFUNCTION()
  void OnMontageEnded(UAnimMontage *anim, bool isInterrupted);

  UFUNCTION()
  void OnMontageBlendingOut(UAnimMontage *anim, bool isInterrupted);

  EShelterMobState state = EShelterMobState::processing;
  class UAnimMontage *AttackMontage;
  class UAnimMontage *DeathMontage;
  class UStaticMeshComponent *mushroomMesh;
};
