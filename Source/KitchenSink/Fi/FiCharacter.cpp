// (c) 2013 Mika Pi

#include "FiCharacter.h"
#include "../Settings.h"
#include "FiCustomer.h"
#include "FiHud.h"
#include "FiHudUi.h"
#include "FiRestaurant.h"
#include <Animation/AnimInstance.h>
#include <Camera/CameraComponent.h>
#include <Components/CapsuleComponent.h>
#include <EnhancedInputComponent.h>
#include <EnhancedInputSubsystems.h>
#include <algorithm>

AFiCharacter::AFiCharacter()
  : Mesh1P(CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"))),
    FirstPersonCameraComponent(CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera")))
{
  GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

  FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
  FirstPersonCameraComponent->SetRelativeLocation(vec(-10.f, 0.f, 60.f)); // Position the camera
  FirstPersonCameraComponent->bUsePawnControlRotation = true;

  Mesh1P->SetOnlyOwnerSee(true);
  Mesh1P->SetupAttachment(FirstPersonCameraComponent);
  Mesh1P->bCastDynamicShadow = false;
  Mesh1P->CastShadow = false;
  Mesh1P->SetRelativeLocation(vec(-30.f, 0.f, -150.f));
}

auto AFiCharacter::BeginPlay() -> void
{
  Super::BeginPlay();

  auto playerController = Cast<APlayerController>(GetController());
  CHECK_RET(playerController);

  auto inputSubsys =
    ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerController->GetLocalPlayer());
  CHECK_RET(inputSubsys);

  inputSubsys->AddMappingContext(DefaultMappingContext, 0);

  auto hudUi = getHudUi();
  CHECK_RET(hudUi);
  hudUi->updateObjective(LOC("Find a restaurant for food pickup"));
  shift = 1.f;
}

auto AFiCharacter::SetupPlayerInputComponent(class UInputComponent *playerInputComp) -> void
{
  auto inputComp = CastChecked<UEnhancedInputComponent>(playerInputComp);
  CHECK_RET(inputComp);
  inputComp->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AFiCharacter::jump);
  inputComp->BindAction(JumpAction, ETriggerEvent::Completed, this, &AFiCharacter::stopJump);
  inputComp->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AFiCharacter::move);
  inputComp->BindAction(LookAction, ETriggerEvent::Triggered, this, &AFiCharacter::look);
  inputComp->BindAction(MainAction, ETriggerEvent::Triggered, this, &AFiCharacter::main);
  updateMouseSensitivity();
}

auto AFiCharacter::move(const FInputActionValue &v) -> void
{
  const auto moveVec = v.Get<FVector2D>();
  CHECK_RET(Controller)
  AddMovementInput(GetActorForwardVector(), moveVec.Y);
  AddMovementInput(GetActorRightVector(), moveVec.X);
}

auto AFiCharacter::look(const FInputActionValue &v) -> void
{
  const auto lookAxisVec = v.Get<FVector2D>();
  CHECK_RET(Controller);
  AddControllerYawInput(lookAxisVec.X * mouseSensitivity);
  AddControllerPitchInput(lookAxisVec.Y * mouseSensitivity);
}

auto AFiCharacter::getHudUi() -> UFiHudUi *
{
  auto playerController = Cast<APlayerController>(GetController());
  CHECK_RET(playerController, nullptr);
  auto hud = Cast<AFiHud>(playerController->GetHUD());
  CHECK_RET(hud, nullptr);
  return hud->getHudUi();
}

auto AFiCharacter::gameOver() -> void
{
  UGameplayStatics::OpenLevel(GetWorld(), FName("FiGameOver"), true, "0");
}

auto AFiCharacter::updateMouseSensitivity() -> void
{
  auto settings = Cast<USettings>(UGameplayStatics::LoadGameFromSlot("settings", 0));
  if (!settings)
    settings = NewObject<USettings>(this, USettings::StaticClass());

  mouseSensitivity = settings->mouseSensitivity;
}

auto AFiCharacter::jump() -> void
{
  if (isJumping)
    return;
  isJumping = true;
  auto movement = GetCharacterMovement();
  const auto v = movement->Velocity.Z;
  const auto m = GetCapsuleComponent()->GetMass();

  movement->AddImpulse(vec(0., 0., std::max(0., -v * m / 250. + 1000.)), true);
}

auto AFiCharacter::stopJump() -> void
{
  isJumping = false;
}

auto AFiCharacter::main() -> void
{
  const auto start = getLoc(this);
  const auto end = start + GetControlRotation().Vector() * 5'00.f;
  FHitResult hitResult;
  auto isHit = GetWorld()->LineTraceSingleByChannel(hitResult, start, end, ECC_Visibility);
  if (!isHit)
  {
    LOG("Not facing anything");
    return;
  }
  const auto hitActor = hitResult.GetActor();
  if (auto r = Cast<AFiRestaurant>(hitActor))
  {
    LOG("Facing restaurant building");

    if (restaurant)
    {
      LOG("We need first to deliver food");
      return;
    }

    const auto world = GetWorld();

    TArray<AActor *> customers;
    UGameplayStatics::GetAllActorsOfClass(world, AFiCustomer::StaticClass(), customers);

    CHECK_RET(customers.Num() != 0);

    r->customer = Cast<AFiCustomer>(customers[FMath::RandRange(0, customers.Num() - 1)]);
    restaurant = r;
    auto hudUi = getHudUi();
    CHECK_RET(hudUi);
    hudUi->updateObjective(LOC("Delivery food to the customer"));

    return;
  }
  if (auto c = Cast<AFiCustomer>(hitActor))
  {
    LOG("Facing customer");
    if (!restaurant)
    {
      LOG("We are not delivering");
      return;
    }
    if (restaurant->customer != c)
    {
      LOG("Wrong customer");
      return;
    }
    ++deliveries;
    const auto curStars = static_cast<int>(
      std::max(0.f, 5.f - log2f(1 + std::max(0.f, (restaurant->getOrderTime() - 25.f) / 25.f))));
    LOG("stars", curStars);
    stars += curStars;
    restaurant->reset();
    restaurant = nullptr;
    auto hudUi = getHudUi();
    CHECK_RET(hudUi);
    hudUi->updateObjective(LOC("Find a restaurant for food pickup"));
    hudUi->showStars(curStars);

    TArray<AActor *> restaurants;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFiRestaurant::StaticClass(), restaurants);
    CHECK_RET(restaurants.Num() != 0);
    const auto hasOrder = [&]() {
      for (const auto a : restaurants)
      {
        const auto r = Cast<AFiRestaurant>(a);
        if (!r)
          continue;
        if (r->hasOrder())
          return true;
      }
      return false;
    }();
    if (!hasOrder)
    {
      LOG("No restaurants having an order, forcing it");
      const auto r = Cast<AFiRestaurant>(restaurants[rand() % restaurants.Num()]);
      CHECK_RET(r);
      r->forceOrder();
    }

    return;
  }
  LOG("Facing something else", hitActor->GetName());
}

auto AFiCharacter::Tick(float dt) -> void
{
  Super::Tick(dt);

  shift -= dt / 300.f;
  updateShift();
  updateHelp();
}

auto AFiCharacter::updateShift() -> void
{
  if (shift <= 0)
  {
    UGameplayStatics::OpenLevel(GetWorld(),
                                FName("FiGameOver"),
                                true,
                                FString::Format(TEXT("deliveries={0}&stars={1}"), {deliveries, stars}));
    return;
  }

  auto hudUi = getHudUi();
  if (!hudUi)
    return;
  hudUi->updateShift(shift);
}

auto AFiCharacter::updateHelp() -> void
{
  auto hudUi = getHudUi();
  if (!hudUi)
    return;

  const auto start = getLoc(this);
  const auto end = start + GetControlRotation().Vector() * 5'00.f;
  FHitResult hitResult;
  auto isHit = GetWorld()->LineTraceSingleByChannel(hitResult, start, end, ECC_Visibility);
  if (!isHit)
  {
    hudUi->updateHelp(FText{});
    return;
  }
  const auto hitActor = hitResult.GetActor();
  if (auto r = Cast<AFiRestaurant>(hitActor))
  {
    if (restaurant)
    {
      hudUi->updateHelp(FText{});
      return;
    }

    hudUi->updateHelp(LOC("RMB - pick up delivery"));
    return;
  }
  if (auto c = Cast<AFiCustomer>(hitActor))
  {
    if (!restaurant)
    {
      hudUi->updateHelp(FText{});
      return;
    }
    if (restaurant->customer != c)
    {
      hudUi->updateHelp(FText{});
      return;
    }
    hudUi->updateHelp(LOC("RMB - delivery food"));
    return;
  }
  hudUi->updateHelp(FText{});
}
