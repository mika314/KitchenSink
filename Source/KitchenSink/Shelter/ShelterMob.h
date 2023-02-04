#pragma once

#include "AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BlackboardData.h"
#include "CoreMinimal.h"
#include "Engine.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"

#include "ShelterMob.generated.h"

UCLASS()
class AShelterMob final : public ACharacter
{
  GENERATED_BODY()
public:
  AShelterMob();

private:
  auto BeginPlay() -> void final;
};
