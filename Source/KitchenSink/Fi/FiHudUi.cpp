// (c) 2023 Mika Pi

#include "FiHudUi.h"

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
