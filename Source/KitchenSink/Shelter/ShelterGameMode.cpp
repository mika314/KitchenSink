// (c) 2023 Mika Pi

#include "ShelterGameMode.h"
#include "ShelterHud.h"

AShelterGameMode::AShelterGameMode()
{
  DefaultPawnClass = CLASS_FINDER(APawn, "1-Shelter", "BP_ShelterCharacter");
  HUDClass = AShelterHud::StaticClass();
}
