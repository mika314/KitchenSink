// (c) 2023 Mika Pi

#pragma once

#include "KitchenSinkCharacter.h"
#include <Components/SphereComponent.h>
#include <CoreMinimal.h>

#include "TP_PickUpComponent.generated.h"

// Declaration of the delegate that will be called when someone picks this up
// The character picking this up is the parameter sent with the notification
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPickUp, AKitchenSinkCharacter *, PickUpCharacter);

UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class KITCHENSINK_API UTP_PickUpComponent final : public USphereComponent
{
  GENERATED_BODY()

public:
  UPROPERTY(BlueprintAssignable, Category = "Interaction")
  FOnPickUp OnPickUp;

  UTP_PickUpComponent();

private:
  auto BeginPlay() -> void final;

  UFUNCTION()
  void OnSphereBeginOverlap(UPrimitiveComponent *OverlappedComponent,
                            AActor *OtherActor,
                            UPrimitiveComponent *OtherComp,
                            int32 OtherBodyIndex,
                            bool bFromSweep,
                            const FHitResult &SweepResult);
};
