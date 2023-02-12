// (c) 2023 Mika Pi

#include "ShelterHudUi.h"

auto UShelterHudUi::setHp(float v) -> void
{
  auto widget = getProp<UProgressBar>(this, TEXT("HpProgress"));
  CHECK_RET(widget);
  widget->SetPercent(v);
}

auto UShelterHudUi::setScrap(int v) -> void
{
  auto widget = getProp<UTextBlock>(this, TEXT("ScrapTb"));
  CHECK_RET(widget);
  widget->SetText(FText::Format(FTextFormat(LOC("{0}")), v));
}

auto UShelterHudUi::setShelterHp(float v) -> void
{
  auto widget = getProp<UProgressBar>(this, TEXT("ShelterHpProgress"));
  CHECK_RET(widget);
  widget->SetPercent(v);
}

auto UShelterHudUi::setMedkits(int v) -> void
{
  auto widget = getProp<UTextBlock>(this, TEXT("MedkitsTb"));
  CHECK_RET(widget);
  widget->SetText(FText::Format(FTextFormat(LOC("{0}")), v));
}

auto UShelterHudUi::mobDied() -> void
{
  --mobs;
  auto widget = getProp<UTextBlock>(this, TEXT("AliensTb"));
  CHECK_RET(widget);
  widget->SetText(FText::Format(FTextFormat(LOC("{0}")), mobs));
}
