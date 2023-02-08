// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "ShelterWeaponComponent.generated.h"
#include <Components/SkeletalMeshComponent.h>
#include <CoreMinimal.h>

class AShelterCharacter;

UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class KITCHENSINK_API UShelterWeaponComponent final : public USkeletalMeshComponent
{
  GENERATED_BODY()

public:
  UShelterWeaponComponent();

  UPROPERTY(EditDefaultsOnly, Category = Projectile)
  TSubclassOf<class AShelterProjectile> ProjectileClass;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
  USoundBase *FireSound;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
  UAnimMontage *FireAnimation;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
  FVector MuzzleOffset;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
  class UInputMappingContext *FireMappingContext;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
  class UInputAction *FireAction;

  UFUNCTION(BlueprintCallable, Category = "Weapon")
  void Fire();

private:
  UFUNCTION()
  void EndPlay(const EEndPlayReason::Type EndPlayReason) final;

public:
  AShelterCharacter *Character;
};
