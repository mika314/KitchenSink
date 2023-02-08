// Copyright Epic Games, Inc. All Rights Reserved.

#include "ShelterProjectile.h"
#include "ShelterMob.h"
#include <Components/SphereComponent.h>
#include <GameFramework/ProjectileMovementComponent.h>

AShelterProjectile::AShelterProjectile()
{
  CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
  CollisionComp->InitSphereRadius(5.0f);
  CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
  CollisionComp->OnComponentHit.AddDynamic(
    this, &AShelterProjectile::OnHit); // set up a notification for when this component hits
                                       // something blocking

  CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
  CollisionComp->CanCharacterStepUpOn = ECB_No;

  RootComponent = CollisionComp;

  ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
  ProjectileMovement->UpdatedComponent = CollisionComp;
  ProjectileMovement->InitialSpeed = 3000.f;
  ProjectileMovement->MaxSpeed = 3000.f;
  ProjectileMovement->bRotationFollowsVelocity = true;
  ProjectileMovement->bShouldBounce = true;

  InitialLifeSpan = 3.0f;
}

void AShelterProjectile::OnHit(UPrimitiveComponent * /*HitComp*/,
                               AActor *OtherActor,
                               UPrimitiveComponent *OtherComp,
                               FVector /*NormalImpulse*/,
                               const FHitResult & /*Hit*/)
{
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
