// (c) 2023 Mika Pi

#include "ShelterGameMode.h"
#include "ShelterHud.h"

AShelterGameMode::AShelterGameMode()
{
  DefaultPawnClass = CLASS_FINDER(APawn, "FirstPerson/Blueprints", "BP_FirstPersonCharacter");
  HUDClass = AShelterHud::StaticClass();
}
