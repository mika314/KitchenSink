// (c) 2023 Mika Pi

#include "PrjHud.h"
#include "HudUi.h"

APrjHud::APrjHud()
  : hudUi(CreateWidget<UHudUi>(GetWorld(), CLASS_FINDER(UHudUi, "1stParty", "BP_HudUi")))
{
}

auto APrjHud::BeginPlay() -> void
{
  Super::BeginPlay();

  CHECK_RET(hudUi);
  hudUi->AddToViewport();

  auto playerController = GetWorld()->GetFirstPlayerController();
  playerController->SetInputMode(FInputModeGameOnly{});
  playerController->bShowMouseCursor = false;
}
