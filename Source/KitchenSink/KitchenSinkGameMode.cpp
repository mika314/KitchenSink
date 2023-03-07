// (c) 2023 Mika Pi

#include "KitchenSinkGameMode.h"
#include "KitchenSinkCharacter.h"
#include "PrjHud.h"
#include <QofL/class_finder.h>

AKitchenSinkGameMode::AKitchenSinkGameMode()
{
  DefaultPawnClass = CLASS_FINDER(APawn, "FirstPerson/Blueprints", "BP_FirstPersonCharacter");
  HUDClass = APrjHud::StaticClass();
}
