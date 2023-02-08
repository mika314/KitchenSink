// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>
#include <GameFramework/Actor.h>

#include "ShelterProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS(config = Game)
class AShelterProjectile final : public AActor
{
  GENERATED_BODY()

  UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
  USphereComponent *CollisionComp;

  UPROPERTY(VisibleAnywhere,
            BlueprintReadOnly,
            Category = Movement,
            meta = (AllowPrivateAccess = "true"))
  UProjectileMovementComponent *ProjectileMovement;

public:
  AShelterProjectile();

  UFUNCTION()
  void OnHit(UPrimitiveComponent *HitComp,
             AActor *OtherActor,
             UPrimitiveComponent *OtherComp,
             FVector NormalImpulse,
             const FHitResult &Hit);

  auto GetCollisionComp() const -> USphereComponent * { return CollisionComp; }
  auto GetProjectileMovement() const -> UProjectileMovementComponent * { return ProjectileMovement; }
};
