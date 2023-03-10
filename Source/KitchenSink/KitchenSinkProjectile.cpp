// Copyright Epic Games, Inc. All Rights Reserved.

#include "KitchenSinkProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Shelter/ShelterMob.h"

AKitchenSinkProjectile::AKitchenSinkProjectile()
{
  // Use a sphere as a simple collision representation
  CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
  CollisionComp->InitSphereRadius(5.0f);
  CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
  CollisionComp->OnComponentHit.AddDynamic(
    this, &AKitchenSinkProjectile::OnHit); // set up a notification for when this component hits
                                           // something blocking

  // Players can't walk on it
  CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
  CollisionComp->CanCharacterStepUpOn = ECB_No;

  // Set as root component
  RootComponent = CollisionComp;

  // Use a ProjectileMovementComponent to govern this projectile's movement
  ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
  ProjectileMovement->UpdatedComponent = CollisionComp;
  ProjectileMovement->InitialSpeed = 3000.f;
  ProjectileMovement->MaxSpeed = 3000.f;
  ProjectileMovement->bRotationFollowsVelocity = true;
  ProjectileMovement->bShouldBounce = true;

  // Die after 3 seconds by default
  InitialLifeSpan = 3.0f;
}

void AKitchenSinkProjectile::OnHit(UPrimitiveComponent * /*HitComp*/,
                                   AActor *OtherActor,
                                   UPrimitiveComponent *OtherComp,
                                   FVector /*NormalImpulse*/,
                                   const FHitResult & /*Hit*/)
{
  // Only add impulse and destroy projectile if we hit a physics
  if (!OtherActor)
    return;
  if (OtherActor == this)
    return;

  if ((OtherComp != nullptr) && OtherComp->IsSimulatingPhysics())
  {
    OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());

    Destroy();
    return;
  }

  if (auto mob = Cast<AShelterMob>(OtherActor))
  {
    mob->die();
    Destroy();
    return;
  }
}
