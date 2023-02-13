// (c) 2023 Mika Pi

#include "ShelterGameOverHudUi.h"

auto UShelterGameOverHudUi::OnWidgetRebuilt() -> void
{
  if (auto gameMode = UGameplayStatics::GetGameMode(this))
  {
    LOG(gameMode->OptionsString);
    auto widget = getProp<UTextBlock>(this, TEXT("GameOverTb"));
    if (gameMode->OptionsString == TEXT("?0"))
      widget->SetText(LOC("You lose.\nTry again."));
    else // if (gameMode->OptionsString == TEXT("?1"))
      widget->SetText(LOC("Victory!"));
  }
  {
    auto widget = getProp<UButton>(this, TEXT("LobbyBtn"));
    CHECK_RET(widget);
    widget->OnClicked.AddDynamic(this, &UShelterGameOverHudUi::onLobbyClicked);
  }
  {
    auto widget = getProp<UButton>(this, TEXT("TryAgainBtn"));
    CHECK_RET(widget);
    widget->OnClicked.AddDynamic(this, &UShelterGameOverHudUi::onTryAgainClicked);
  }
}

auto UShelterGameOverHudUi::onTryAgainClicked() -> void
{
  UGameplayStatics::OpenLevel(GetWorld(), FName("Shelter"), true);
}

auto UShelterGameOverHudUi::onLobbyClicked() -> void
{
  UGameplayStatics::OpenLevel(GetWorld(), FName("FirstPersonMap"), true);
}
