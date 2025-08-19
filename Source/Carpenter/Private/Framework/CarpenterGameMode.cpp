// Copyright Epic Games, Inc. All Rights Reserved.

#include "Framework/CarpenterGameMode.h"

#include "UObject/ConstructorHelpers.h"

ACarpenterGameMode::ACarpenterGameMode() : Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;
}
