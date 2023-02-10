// (c) 2023 Mika Pi

#include "ShelterHudUi.h"

auto UShelterHudUi::setHp(float v) -> void
{
  auto progress = getProp<UProgressBar>(this, TEXT("HpProgress"));
  CHECK_RET(progress);
  progress->SetPercent(v);
}

auto UShelterHudUi::setScrap(int v) -> void
{
  auto widget = getProp<UTextBlock>(this, TEXT("ScrapTb"));
  CHECK_RET(widget);
  widget->SetText(FText::Format(FTextFormat(LOC("{0}")), v));
}
