// (c) 2023 Mika Pi

#include "FiGameOverHud.h"
#include "FiGameOverHudUi.h"

AFiGameOverHud::AFiGameOverHud()
  : hudUi(CreateWidget<UFiGameOverHudUi>(
      GetWorld(),
      CLASS_FINDER(UFiGameOverHudUi, "2-FloatingIslands", "BP_FiGameOverHudUi")))
{
}

auto AFiGameOverHud::BeginPlay() -> void
{
  Super::BeginPlay();
  CHECK_RET(hudUi);
  hudUi->AddToViewport();
  auto playerController = GetWorld()->GetFirstPlayerController();
  playerController->SetInputMode(FInputModeUIOnly{});
  playerController->bShowMouseCursor = true;
}
