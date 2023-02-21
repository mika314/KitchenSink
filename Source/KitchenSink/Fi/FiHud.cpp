// (c) 2023 Mika Pi

#include "FiHud.h"
#include "FiHudUi.h"

AFiHud::AFiHud()
  : hudUi(CreateWidget<UFiHudUi>(GetWorld(), CLASS_FINDER(UFiHudUi, "2-FloatingIslands", "BP_FiHudUi")))
{
}

auto AFiHud::BeginPlay() -> void
{
  Super::BeginPlay();

  CHECK_RET(hudUi);
  hudUi->AddToViewport();

  auto playerController = GetWorld()->GetFirstPlayerController();
  playerController->SetInputMode(FInputModeGameOnly{});
  playerController->bShowMouseCursor = false;
}
