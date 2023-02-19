// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>
#include <GameFramework/Character.h>
#include <InputActionValue.h>

#include "KitchenSinkCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class UAnimMontage;
class USoundBase;

UCLASS(config = Game)
class AKitchenSinkCharacter : public ACharacter
{
  GENERATED_BODY()

  /** Pawn mesh: 1st person view (arms; seen only by self) */
  UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
  USkeletalMeshComponent *Mesh1P;

  /** First person camera */
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
  UCameraComponent *FirstPersonCameraComponent;

  /** MappingContext */
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
  class UInputMappingContext *DefaultMappingContext;

  /** Jump Input Action */
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
  class UInputAction *JumpAction;

  /** Move Input Action */
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
  class UInputAction *MoveAction;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
  class UInputAction *SettingsAction;

public:
  AKitchenSinkCharacter();

  /** Look Input Action */
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
  class UInputAction *LookAction;

  /** Bool for AnimBP to switch to another animation set */
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
  bool bHasRifle;

  /** Setter to set the bool */
  UFUNCTION(BlueprintCallable, Category = Weapon)
  void SetHasRifle(bool bNewHasRifle);

  /** Getter for the bool */
  UFUNCTION(BlueprintCallable, Category = Weapon)
  bool GetHasRifle();

private:
  auto BeginPlay() -> void final;
  auto SetupPlayerInputComponent(UInputComponent *) -> void final;
  auto look(const FInputActionValue &) -> void;
  auto move(const FInputActionValue &) -> void;
  auto settings() -> void;

public:
  /** Returns Mesh1P subobject **/
  USkeletalMeshComponent *GetMesh1P() const { return Mesh1P; }
  /** Returns FirstPersonCameraComponent subobject **/
  UCameraComponent *GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }
};
