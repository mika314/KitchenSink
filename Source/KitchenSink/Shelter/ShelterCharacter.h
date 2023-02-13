// (c) 2013 Mika Pi

#pragma once

#include <CoreMinimal.h>
#include <GameFramework/Character.h>
#include <InputActionValue.h>

#include "ShelterCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class UAnimMontage;
class USoundBase;

UCLASS(config = Game)
class AShelterCharacter final : public ACharacter
{
  GENERATED_BODY()

public:
  AShelterCharacter();
  auto addMedkit() -> void;
  auto addScrap() -> void;
  auto applyDamage(float) -> void;
  auto applyShelterDamage(float) -> void;
  auto getHp() const -> float;
  auto getHudUi() -> class UShelterHudUi *;
  auto getMesh1P() const -> USkeletalMeshComponent * { return Mesh1P; }
  auto getScrap() const -> int;
  auto getShelterHp() const -> float;

private:
  UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
  USkeletalMeshComponent *Mesh1P;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
  UCameraComponent *FirstPersonCameraComponent;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
  class UShelterWeaponComponent *shelterWeapon = nullptr;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
  class UInputMappingContext *DefaultMappingContext;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
  class UInputAction *JumpAction;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
  class UInputAction *MoveAction;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
  class UInputAction *LookAction;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
  class UInputAction *HealAction;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
  class UInputAction *RepairAction;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
  class UInputAction *PlaceTowerAction;

  UFUNCTION(BlueprintCallable, Category = Weapon)
  bool GetHasRifle() const { return true; };

  float hp;
  float shelterHp;
  int scrap;
  int medkits;
  USoundBase *medkitSnd;
  USoundBase *scrapSnd;
  USoundBase *towerSnd;
  USoundBase *healingSnd;

  auto BeginPlay() -> void final;
  auto SetupPlayerInputComponent(UInputComponent *InputComponent) -> void final;
  auto gameOver() -> void;
  auto heal() -> void;
  auto look(const FInputActionValue &Value) -> void;
  auto move(const FInputActionValue &Value) -> void;
  auto placeTower() -> void;
  auto repair() -> void;
};
