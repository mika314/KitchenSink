// (c) 2023 Mika Pi

#pragma once

#include <CoreMinimal.h>
#include <Engine/StaticMeshActor.h>

#include "FiRestaurant.generated.h"

UCLASS()
class KITCHENSINK_API AFiRestaurant final : public AStaticMeshActor
{
  GENERATED_BODY()
public:
  AFiRestaurant();
  auto forceOrder() -> void;
  auto getOrderTime() const -> float { return orderTime; }
  auto hasOrder() const -> bool;
  auto reset() -> void;

  class AFiCustomer *customer = nullptr;

  UPROPERTY(EditAnywhere, BlueprintReadOnly)
  bool instantOrder = false;

private:
  auto BeginPlay() -> void final;
  auto Tick(float) -> void final;

  float orderTime = -1.f;
  float nextRandCheck = 0.f;
  class UTextRenderComponent *text;
};
