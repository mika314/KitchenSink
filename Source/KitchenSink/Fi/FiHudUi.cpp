// (c) 2023 Mika Pi

#include "FiHudUi.h"
#include <Animation/WidgetAnimation.h>

auto UFiHudUi::updateHelp(FText v) -> void
{
  auto widget = getProp<UTextBlock>(this, TEXT("HelpTb"));
  CHECK_RET(widget);
  widget->SetText(std::move(v));
}

auto UFiHudUi::update(Objective v) -> void
{
  auto widget = getProp<UTextBlock>(this, TEXT("ObjectiveTb"));
  CHECK_RET(widget);
  auto bagImg = getProp<UImage>(this, TEXT("BagImg"));
  CHECK_RET(bagImg);
  switch (v)
  {
  case Objective::pickup:
    widget->SetText(LOC("Find a restaurant for food pickup"));
    bagImg->SetVisibility(ESlateVisibility::Hidden);
    break;
  case Objective::delivery:
    widget->SetText(LOC("Delivery food to the customer"));
    bagImg->SetVisibility(ESlateVisibility::Visible);
    break;
  }
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

auto UFiHudUi::updateIncome(int v) -> void
{
  auto widget = getProp<UTextBlock>(this, TEXT("IncomeTb"));
  CHECK_RET(widget);
  widget->SetText(FText::Format(LOC("${0}"), v));
}
