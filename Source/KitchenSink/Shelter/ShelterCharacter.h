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
  auto GetMesh1P() const -> USkeletalMeshComponent * { return Mesh1P; }
  auto getHp() const -> float;
  auto applyDamage(float) -> void;
  auto applyShelterDamage(float) -> void;
  auto addScrap() -> void;

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

  UFUNCTION(BlueprintCallable, Category = Weapon)
  bool GetHasRifle() const { return true; };

  float hp;
  float shelterHp;
  int scrap;

  auto BeginPlay() -> void final;
  auto GetFirstPersonCameraComponent() const -> UCameraComponent * { return FirstPersonCameraComponent; }
  auto Look(const FInputActionValue &Value) -> void;
  auto Move(const FInputActionValue &Value) -> void;
  auto SetupPlayerInputComponent(UInputComponent *InputComponent) -> void final;
};
