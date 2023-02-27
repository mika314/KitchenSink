// (c) 2013 Mika Pi

#pragma once

#include <CoreMinimal.h>
#include <GameFramework/Character.h>
#include <InputActionValue.h>

#include "FiCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class UAnimMontage;
class USoundBase;

UCLASS(config = Game)
class AFiCharacter final : public ACharacter
{
  GENERATED_BODY()

public:
  AFiCharacter();
  auto getHudUi() -> class UFiHudUi *;
  auto getMesh1P() const -> USkeletalMeshComponent * { return Mesh1P; }
  auto getRestaurant() const -> class AFiRestaurant * { return restaurant; }

private:
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
  class UInputAction *MainAction;

  float mouseSensitivity = 1.f;
  bool isJumping = false;
  class AFiRestaurant *restaurant = nullptr;

  auto BeginPlay() -> void final;
  auto SetupPlayerInputComponent(UInputComponent *InputComponent) -> void final;
  auto Tick(float) -> void final;
  auto gameOver() -> void;
  auto jump() -> void;
  auto look(const FInputActionValue &Value) -> void;
  auto main() -> void;
  auto move(const FInputActionValue &Value) -> void;
  auto stopJump() -> void;
  auto updateHelp() -> void;
  auto updateMouseSensitivity() -> void;
};
