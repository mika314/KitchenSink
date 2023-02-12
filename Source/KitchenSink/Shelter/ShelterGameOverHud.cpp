// (c) 2023 Mika Pi

#include "ShelterGameOverHud.h"
#include "ShelterGameOverHudUi.h"

AShelterGameOverHud::AShelterGameOverHud()
  : hudUi(CreateWidget<UShelterGameOverHudUi>(
      GetWorld(),
      CLASS_FINDER(UShelterGameOverHudUi, "1-Shelter", "BP_ShelterGameOverHudUi")))
{
}

auto AShelterGameOverHud::BeginPlay() -> void
{
  Super::BeginPlay();
  CHECK_RET(hudUi);
  hudUi->AddToViewport();

  auto playerController = GetWorld()->GetFirstPlayerController();
  FInputModeUIOnly inputMode;
  inputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
  inputMode.SetWidgetToFocus(nullptr);
  playerController->SetInputMode(inputMode);
  playerController->bShowMouseCursor = true;
}
