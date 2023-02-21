// (c) 2023 Mika Pi

#include "FiGameMode.h"
#include "FiHud.h"

AFiGameMode::AFiGameMode()
{
  DefaultPawnClass = CLASS_FINDER(APawn, "2-FloatingIslands", "BP_FiCharacter");
  HUDClass = AFiHud::StaticClass();
}
