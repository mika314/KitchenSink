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
  auto lineTraceToDetermineHit() -> void;
  auto processState() -> void;
  auto setupAi() -> void;

  UFUNCTION()
  void onMoveToActorFinished(FAIRequestID reqId, EPathFollowingResult::Type result);

  UFUNCTION()
  void onMontageEnded(UAnimMontage *anim, bool isInterrupted);

  UFUNCTION()
  void onMontageBlendingOut(UAnimMontage *anim, bool isInterrupted);

  EShelterMobState state = EShelterMobState::processing;
  class UAnimMontage *attackMontage;
  class UAnimMontage *deathMontage;
  class UStaticMeshComponent *mushroomMesh;
  class USoundBase *sndAttack;
  class USoundBase *sndDeath;
};
