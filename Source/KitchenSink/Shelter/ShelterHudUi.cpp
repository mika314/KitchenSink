// (c) 2023 Mika Pi

#include "ShelterHudUi.h"

auto UShelterHudUi::setHp(float v) -> void
{
  auto progress = getProp<UProgressBar>(this, TEXT("HpProgress"));
  CHECK_RET(progress);
  progress->SetPercent(v);
}
