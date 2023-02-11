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
private:
  UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
  USphereComponent *CollisionComp;

  UPROPERTY(VisibleAnywhere,
            BlueprintReadOnly,
            Category = Movement,
            meta = (AllowPrivateAccess = "true"))
  UProjectileMovementComponent *ProjectileMovement;

public:
  AShelterProjectile();
  auto getCollisionComp() const -> USphereComponent * { return CollisionComp; }
  auto getProjectileMovement() const -> UProjectileMovementComponent * { return ProjectileMovement; }

  UFUNCTION()
  void onHit(UPrimitiveComponent *hitComp,
             AActor *otherActor,
             UPrimitiveComponent *otherComp,
             FVector normalImpulse,
             const FHitResult &hit);
};
