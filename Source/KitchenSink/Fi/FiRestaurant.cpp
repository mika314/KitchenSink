// (c) 2023 Mika Pi

#include "FiRestaurant.h"

AFiRestaurant::AFiRestaurant() : text(CreateDefaultSubobject<UTextRenderComponent>(TEXT("text")))
{
  auto mesh = GetStaticMeshComponent();
  mesh->SetStaticMesh(OBJ_FINDER(StaticMesh, "StarterContent/Props", "SM_CornerFrame"));
  mesh->SetWorldScale3D(vec(10., 10., 10.));
  mesh->SetMobility(EComponentMobility::Movable);
  text->SetupAttachment(mesh);
  text->SetVisibleFlag(false);
  text->SetHorizontalAlignment(EHTA_Center);
  PrimaryActorTick.bCanEverTick = true;
}

auto AFiRestaurant::hasOrder() const -> bool
{
  return orderTime >= 0.f;
}

auto AFiRestaurant::BeginPlay() -> void
{
  Super::BeginPlay();
  orderTime = -1.f;
  nextRandCheck = 0;
}

auto AFiRestaurant::Tick(float dt) -> void
{
  Super::Tick(dt);
  if (orderTime < 0.f)
  {
    text->SetVisibleFlag(false);
    if (GetWorld()->GetTimeSeconds() > nextRandCheck)
    {
      nextRandCheck += 1.f;
      if (rand() % 10 == 0)
      {
        orderTime = GetWorld()->GetTimeSeconds();
        LOG("new order");
      }
    }
  }
  else
  {
    text->SetVisibleFlag(true);

    FNumberFormattingOptions opt;
    opt.SetMaximumFractionalDigits(1);
    opt.SetMinimumFractionalDigits(1);
    text->SetText(
      FText::Format(LOC("{0} s"), FText::AsNumber(GetWorld()->GetTimeSeconds() - orderTime, &opt)));
    const auto character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    const auto l = getLoc(character);
    const auto dist = getLoc(this) - l;
    const auto norm = dist.GetSafeNormal(0.01);
    text->SetWorldRotation(FRotationMatrix::MakeFromX(-norm).Rotator());
    if (dist.Size() > 4000.)
      text->SetWorldLocation(l + norm * 4000. + vec(0., 0., 1000.));
    else
      text->SetWorldLocation(getLoc(this) + vec(0., 0., 1000.));
  }
}
