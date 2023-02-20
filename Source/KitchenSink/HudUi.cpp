// (c) 2023 Mika Pi

#include "HudUi.h"
#include "KitchenSinkCharacter.h"
#include "SettingsUi.h"

auto UHudUi::toggleSettings() -> void
{
  auto switcher = getProp<UWidgetSwitcher>(this, TEXT("WidgetSwitcher"));
  CHECK_RET(switcher);
  auto settings = getProp<USettingsUi>(this, TEXT("Settings"));
  CHECK_RET(settings);

  if (switcher->GetActiveWidget() != settings)
  {
    switcher->SetActiveWidget(settings);
    auto playerController = GetWorld()->GetFirstPlayerController();
    playerController->SetInputMode(FInputModeGameAndUI{});
    playerController->bShowMouseCursor = true;
  }
  else
  {
    auto gamePlay = getProp<UCanvasPanel>(this, TEXT("GamePlay"));
    CHECK_RET(gamePlay);
    switcher->SetActiveWidget(gamePlay);
    auto playerController = GetWorld()->GetFirstPlayerController();
    playerController->SetInputMode(FInputModeGameOnly{});
    playerController->bShowMouseCursor = false;
    auto character = Cast<AKitchenSinkCharacter>(playerController->GetPawnOrSpectator());
    CHECK_RET(character);
    character->updateMouseSensitivity();
  }
}
