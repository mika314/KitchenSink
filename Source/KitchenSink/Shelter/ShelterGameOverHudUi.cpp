// (c) 2023 Mika Pi

#include "ShelterGameOverHudUi.h"

auto UShelterGameOverHudUi::OnWidgetRebuilt() -> void
{
  Super::OnWidgetRebuilt();
  if (auto gameMode = UGameplayStatics::GetGameMode(this))
  {
    LOG(gameMode->OptionsString);
    if (gameMode->OptionsString == TEXT("?0"))
    {
      auto widget = getProp<UTextBlock>(this, TEXT("GameOverTb"));
      CHECK_RET(widget);
      widget->SetText(LOC("You lose.\nTry again."));
    }
    else
    {
      {
        auto widget = getProp<UTextBlock>(this, TEXT("GameOverTb"));
        CHECK_RET(widget);
        widget->SetText(LOC("Victory!"));
      }
      {
        auto widget = getProp<UTextBlock>(this, TEXT("TimeTb"));
        CHECK_RET(widget);
        const auto time = FCString::Atoi(*gameMode->OptionsString.RightChop(1)) / 10.f;
        widget->SetText(FText::Format(LOC("{0:%.1f} s"), time));
      }
    }
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
