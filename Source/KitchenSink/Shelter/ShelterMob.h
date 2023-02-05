#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "ShelterMob.generated.h"

UENUM(BlueprintType)
enum class EShelterMobState : uint8 {
  busy,
  patrolling,
  attacking,
  processing,
};

UCLASS()
class AShelterMob final : public ACharacter
{
  GENERATED_BODY()
public:
  AShelterMob();

  UFUNCTION(BlueprintCallable)
  EShelterMobState getState() const;

private:
  auto BeginPlay() -> void final;
  auto Tick(float) -> void final;
  auto processState() -> void;

  UFUNCTION()
  void OnMoveToActorFinished(FAIRequestID reqId, EPathFollowingResult::Type Result);

  UFUNCTION()
  void OnMontageEnded(UAnimMontage *anim, bool isInterrupted);

  EShelterMobState state = EShelterMobState::patrolling;
  class UAnimMontage *AttackMontage;
};
