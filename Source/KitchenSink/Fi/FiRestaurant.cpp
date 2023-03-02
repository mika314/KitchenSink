// (c) 2023 Mika Pi

#include "FiRestaurant.h"
#include "FiCharacter.h"
#include "FiCustomer.h"

AFiRestaurant::AFiRestaurant()
  : text(CreateDefaultSubobject<UTextRenderComponent>(TEXT("text"))),
    newOrderSnd(OBJ_FINDER(SoundCue, "2-FloatingIslands", "SND_Bell_Cue"))
{
  auto mesh = GetStaticMeshComponent();
  mesh->SetStaticMesh(OBJ_FINDER(StaticMesh, "StarterContent/Props", "SM_CornerFrame"));
  mesh->SetWorldScale3D(vec(10., 10., 10.));
  mesh->SetMobility(EComponentMobility::Movable);
  text->SetupAttachment(mesh);
  text->SetVisibleFlag(false);
  text->SetText(FText{});
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
  nextRandCheck = 0.5f;
  customer = nullptr;
}

auto AFiRestaurant::Tick(float dt) -> void
{
  Super::Tick(dt);
  const auto character = Cast<AFiCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
  CHECK_RET(character);
  if (orderTime < 0.f)
  {
    text->SetVisibleFlag(false);
    text->SetText(FText{});
    if (GetWorld()->GetTimeSeconds() > nextRandCheck)
    {
      nextRandCheck += 1.f;
      if (!character->getRestaurant() && (rand() % 200 == 0 || instantOrder))
      {
        instantOrder = false;
        orderTime = GetWorld()->GetTimeSeconds();
        UGameplayStatics::PlaySoundAtLocation(GetWorld(), newOrderSnd, getLoc(this));
        LOG("new order");
      }
    }
    return;
  }

  if (!character->getRestaurant())
  {
    text->SetVisibleFlag(true);

    FNumberFormattingOptions opt;
    opt.SetMaximumFractionalDigits(1);
    opt.SetMinimumFractionalDigits(1);
    text->SetText(
      FText::Format(LOC("{0} s"), FText::AsNumber(GetWorld()->GetTimeSeconds() - orderTime, &opt)));
    const auto l = getLoc(character);
    const auto dist = getLoc(this) - l;
    const auto norm = dist.GetSafeNormal(0.01);
    text->SetWorldRotation(FRotationMatrix::MakeFromX(-norm).Rotator());
    if (dist.Size() > 4000.)
      text->SetWorldLocation(l + norm * 4000. + vec(0., 0., 1000.));
    else
      text->SetWorldLocation(getLoc(this) + vec(0., 0., 1000.));
    return;
  }

  if (character->getRestaurant() == this)
  {
    FNumberFormattingOptions opt;
    opt.SetMaximumFractionalDigits(1);
    opt.SetMinimumFractionalDigits(1);
    text->SetText(
      FText::Format(LOC("{0} s"), FText::AsNumber(GetWorld()->GetTimeSeconds() - orderTime, &opt)));
    const auto l = getLoc(character);
    const auto dist = getLoc(customer) - l;
    const auto norm = dist.GetSafeNormal(0.01);
    text->SetWorldRotation(FRotationMatrix::MakeFromX(-norm).Rotator());
    if (dist.Size() > 4000.)
      text->SetWorldLocation(l + norm * 4000. + vec(0., 0., 1000.));
    else
      text->SetWorldLocation(getLoc(customer) + vec(0., 0., 1000.));
    return;
  }
  text->SetVisibleFlag(false);
  text->SetText(FText{});
}

auto AFiRestaurant::reset() -> void
{
  orderTime = -1.f;
  customer = nullptr;
}

auto AFiRestaurant::forceOrder() -> void
{
  instantOrder = true;
  orderTime = -1.f;
  nextRandCheck = 0;
}
