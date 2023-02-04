// Copyright Epic Games, Inc. All Rights Reserved.

#include "KitchenSinkGameMode.h"
#include "KitchenSinkCharacter.h"
#include "UObject/ConstructorHelpers.h"

AKitchenSinkGameMode::AKitchenSinkGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
