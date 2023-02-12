// (c) 2023 Mika Pi

#include "ShelterHud.h"
#include "ShelterHudUi.h"

AShelterHud::AShelterHud()
  : hudUi(CreateWidget<UShelterHudUi>(GetWorld(),
                                      CLASS_FINDER(UShelterHudUi, "1-Shelter", "BP_ShelterHudUi")))
{
}

auto AShelterHud::BeginPlay() -> void
{
  Super::BeginPlay();

  CHECK_RET(hudUi);
  hudUi->AddToViewport();

  auto playerController = GetWorld()->GetFirstPlayerController();
  playerController->SetInputMode(FInputModeGameOnly{});
  playerController->bShowMouseCursor = false;
}
