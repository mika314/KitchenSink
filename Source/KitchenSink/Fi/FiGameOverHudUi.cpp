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
  // Assume we have a string called OptionsString containing the options
  FString OptionsString = gameMode->OptionsString;

  // Remove the leading question mark from the string
  OptionsString.RemoveAt(0, 1);

  // Split the options string into an array of key-value pairs
  TArray<FString> KeyValuePairs;
  OptionsString.ParseIntoArray(KeyValuePairs, TEXT("&"));

  // Loop through each key-value pair and parse it as needed
  int deliveries = 0;
  int stars = 0;
  int income = 0;
  for (FString KeyValuePair : KeyValuePairs)
  {
    FString Key, Value;
    if (KeyValuePair.Split(TEXT("="), &Key, &Value))
    {
      // Do something with the key and value here
      if (Key == "deliveries")
        deliveries = FCString::Atoi(*Value);
      else if (Key == "stars")
        stars = FCString::Atoi(*Value);
      else if (Key == "income")
        income = FCString::Atoi(*Value);
    }
  }
  {
    auto widget = getProp<UTextBlock>(this, TEXT("customersTb"));
    CHECK_RET(widget);
    widget->SetText(FText::Format(LOC("Customers: {0}"), deliveries));
  }
  {
    auto widget = getProp<UProgressBar>(this, TEXT("starsPb"));
    CHECK_RET(widget);
    if (deliveries > 0)
      widget->SetPercent(1.f * stars / deliveries / 5.f);
    else
      widget->SetPercent(0.f);
  }

  {
    auto widget = getProp<UTextBlock>(this, TEXT("incomeTb"));
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
