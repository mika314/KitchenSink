// (c) 2023 Mika Pi

#include "TP_PickUpComponent.h"
#include "KitchenSinkCharacter.h"

UTP_PickUpComponent::UTP_PickUpComponent()
{
  SphereRadius = 32.f;
}

auto UTP_PickUpComponent::BeginPlay() -> void
{
  Super::BeginPlay();
  OnComponentBeginOverlap.AddDynamic(this, &UTP_PickUpComponent::OnSphereBeginOverlap);
}

auto UTP_PickUpComponent::OnSphereBeginOverlap(UPrimitiveComponent * /*OverlappedComponent*/,
                                               AActor *OtherActor,
                                               UPrimitiveComponent * /*OtherComp*/,
                                               int32 /*OtherBodyIndex*/,
                                               bool /*bFromSweep*/,
                                               const FHitResult & /*SweepResult*/) -> void
{
  if (auto Character = Cast<AKitchenSinkCharacter>(OtherActor))
  {
    OnPickUp.Broadcast(Character);
    OnComponentBeginOverlap.RemoveAll(this);
  }
}
