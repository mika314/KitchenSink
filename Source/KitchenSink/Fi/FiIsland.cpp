// (c) 2023 Mika Pi

#include "FiIsland.h"

AFiIsland::AFiIsland()
{
  auto mesh = GetStaticMeshComponent();
  mesh->SetStaticMesh(OBJ_FINDER(StaticMesh, "2-FloatingIslands", "SM_FloatingIsland"));
  mesh->SetMobility(EComponentMobility::Movable);
  PrimaryActorTick.bCanEverTick = true;
}

auto AFiIsland::BeginPlay() -> void
{
  Super::BeginPlay();
  initLoc = getLoc(this);
}

auto AFiIsland::Tick(float dt) -> void
{
  Super::Tick(dt);
  setLoc(this,
         initLoc + vec(0.f,
                       0.f,
                       50.f * sinf(initLoc.X + initLoc.Y +
                                   2.f * 3.14159f * GetWorld()->GetTimeSeconds() /
                                     (4.f + sinf(initLoc.X + initLoc.Y)))));
}
