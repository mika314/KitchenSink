// (c) 2023 Mika Pi

#include "FiGameOverHudUi.h"

auto UFiGameOverHudUi::OnWidgetRebuilt() -> void
{
  Super::OnWidgetRebuilt();
  {
    auto widget = getProp<UButton>(this, TEXT("LobbyBtn"));
    CHECK_RET(widget);
    widget->OnClicked.AddDynamic(this, &UFiGameOverHudUi::onLobbyClicked);
  }
  {
    auto widget = getProp<UButton>(this, TEXT("TryAgainBtn"));
    CHECK_RET(widget);
    widget->OnClicked.AddDynamic(this, &UFiGameOverHudUi::onTryAgainClicked);
  }

  auto gameMode = UGameplayStatics::GetGameMode(this);
  CHECK_RET(gameMode);
  LOG(gameMode->OptionsString);
  auto optionsString = gameMode->OptionsString;
  optionsString.RemoveAt(0, 1);
  TArray<FString> keyValuePairs;
  optionsString.ParseIntoArray(keyValuePairs, TEXT("&"));

  int customers = 0;
  int stars = 0;
  int income = 0;
  for (const auto &keyValuePair : keyValuePairs)
  {
    FString key, value;
    if (!keyValuePair.Split(TEXT("="), &key, &value))
      continue;
    if (key == "customers")
      customers = FCString::Atoi(*value);
    else if (key == "stars")
      stars = FCString::Atoi(*value);
    else if (key == "income")
      income = FCString::Atoi(*value);
  }
  {
    auto widget = getProp<UTextBlock>(this, TEXT("CustomersTb"));
    CHECK_RET(widget);
    widget->SetText(FText::Format(LOC("Customers: {0}"), customers));
  }
  {
    auto widget = getProp<UProgressBar>(this, TEXT("StarsPb"));
    CHECK_RET(widget);
    if (customers > 0)
      widget->SetPercent(1.f * stars / customers / 5.f);
    else
      widget->SetPercent(0.f);
  }

  {
    auto widget = getProp<UTextBlock>(this, TEXT("IncomeTb"));
    CHECK_RET(widget);
    widget->SetText(FText::Format(LOC("Income: ${0}"), income));
  }
}

auto UFiGameOverHudUi::onTryAgainClicked() -> void
{
  UGameplayStatics::OpenLevel(GetWorld(), FName("FiMap"), true);
}

auto UFiGameOverHudUi::onLobbyClicked() -> void
{
  UGameplayStatics::OpenLevel(GetWorld(), FName("FirstPersonMap"), true);
}
