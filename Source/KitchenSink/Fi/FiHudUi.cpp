// (c) 2023 Mika Pi

#include "FiHudUi.h"
#include <Animation/WidgetAnimation.h>

auto UFiHudUi::updateHelp(FText v) -> void
{
  auto widget = getProp<UTextBlock>(this, TEXT("HelpTb"));
  CHECK_RET(widget);
  widget->SetText(std::move(v));
}

auto UFiHudUi::updateObjective(FText v) -> void
{
  auto widget = getProp<UTextBlock>(this, TEXT("ObjectiveTb"));
  CHECK_RET(widget);
  widget->SetText(std::move(v));
}

auto UFiHudUi::updateShift(float v) -> void
{
  auto widget = getProp<UProgressBar>(this, TEXT("ShiftPb"));
  CHECK_RET(widget);
  widget->SetPercent(v);
}

auto UFiHudUi::showStars(int v) -> void
{
  auto widget = getProp<UProgressBar>(this, TEXT("StarsPb"));
  CHECK_RET(widget);
  widget->SetPercent(v / 5.f);
  auto anim = getProp<UWidgetAnimation>(this, TEXT("Stars"));
  CHECK_RET(anim);
  PlayAnimation(anim, 0);
}
