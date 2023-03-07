// (c) 2023 Mika Pi

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

  UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
  USkeletalMeshComponent *Mesh1P;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
  UCameraComponent *FirstPersonCameraComponent;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
  class UInputMappingContext *DefaultMappingContext;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
  class UInputAction *JumpAction;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
  class UInputAction *MoveAction;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
  class UInputAction *LookAction;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
  class UInputAction *SettingsAction;

public:
  AKitchenSinkCharacter();
  auto GetFirstPersonCameraComponent() -> UCameraComponent * { return FirstPersonCameraComponent; }
  auto GetFirstPersonCameraComponent() const -> const UCameraComponent *;
  auto GetMesh1P() -> USkeletalMeshComponent * { return Mesh1P; }
  auto GetMesh1P() const -> const USkeletalMeshComponent * { return Mesh1P; }
  auto updateMouseSensitivity() -> void;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
  bool bHasRifle;

  UFUNCTION(BlueprintCallable, Category = Weapon)
  void SetHasRifle(bool v);

  UFUNCTION(BlueprintCallable, Category = Weapon)
  bool GetHasRifle();

private:
  auto BeginPlay() -> void final;
  auto SetupPlayerInputComponent(UInputComponent *) -> void final;
  auto look(const FInputActionValue &) -> void;
  auto move(const FInputActionValue &) -> void;
  auto settings() -> void;

  float mouseSensitivity = 1.f;
};
