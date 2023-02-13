// (c) 2013 Mika Pi

#include "ShelterCharacter.h"
#include "RepairAmount.h"
#include "ShelterHud.h"
#include "ShelterHudUi.h"
#include "ShelterShelter.h"
#include "ShelterTower.h"
#include "ShelterWeaponComponent.h"
#include <Animation/AnimInstance.h>
#include <Camera/CameraComponent.h>
#include <Components/CapsuleComponent.h>
#include <EnhancedInputComponent.h>
#include <EnhancedInputSubsystems.h>

AShelterCharacter::AShelterCharacter()
  : Mesh1P(CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"))),
    FirstPersonCameraComponent(CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"))),
    shelterWeapon(CreateDefaultSubobject<UShelterWeaponComponent>(TEXT("shelterWeapon"))),
    medkitSnd(OBJ_FINDER(SoundCue, "1-Shelter/Snd", "SND_Medkit_Cue")),
    scrapSnd(OBJ_FINDER(SoundCue, "1-Shelter/Snd", "SND_Scrap01_Cue")),
    towerSnd(OBJ_FINDER(SoundCue, "1-Shelter/Snd", "SND_Tower_Cue")),
    healingSnd(OBJ_FINDER(SoundCue, "1-Shelter/Snd", "SND_Healing_Cue"))
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

  FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
  shelterWeapon->SetupAttachment(Mesh1P, FName(TEXT("GripPoint")));
  shelterWeapon->character = this;
}

auto AShelterCharacter::BeginPlay() -> void
{
  Super::BeginPlay();

  auto playerController = Cast<APlayerController>(GetController());
  CHECK_RET(playerController);

  auto inputSubsys =
    ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerController->GetLocalPlayer());
  CHECK_RET(inputSubsys);

  inputSubsys->AddMappingContext(DefaultMappingContext, 0);

  CHECK_RET(shelterWeapon);
  inputSubsys->AddMappingContext(shelterWeapon->FireMappingContext, 1);

  auto inputComp = Cast<UEnhancedInputComponent>(playerController->InputComponent);
  CHECK_RET(inputComp);

  inputComp->BindAction(
    shelterWeapon->FireAction, ETriggerEvent::Triggered, shelterWeapon, &UShelterWeaponComponent::Fire);

  hp = 1.f;
  shelterHp = 1.f;
  scrap = 2;
  medkits = 1;
  auto hudUi = getHudUi();
  CHECK_RET(hudUi);
  hudUi->setHp(getHp());
  hudUi->setShelterHp(shelterHp);
  hudUi->setScrap(scrap);
  hudUi->setMedkits(medkits);
}

auto AShelterCharacter::SetupPlayerInputComponent(class UInputComponent *playerInputComp) -> void
{
  auto inputComp = CastChecked<UEnhancedInputComponent>(playerInputComp);
  CHECK_RET(inputComp);
  inputComp->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
  inputComp->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
  inputComp->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AShelterCharacter::move);
  inputComp->BindAction(LookAction, ETriggerEvent::Triggered, this, &AShelterCharacter::look);
  inputComp->BindAction(HealAction, ETriggerEvent::Triggered, this, &AShelterCharacter::heal);
  inputComp->BindAction(RepairAction, ETriggerEvent::Triggered, this, &AShelterCharacter::repair);
  inputComp->BindAction(
    PlaceTowerAction, ETriggerEvent::Triggered, this, &AShelterCharacter::placeTower);
}

auto AShelterCharacter::move(const FInputActionValue &v) -> void
{
  const auto moveVec = v.Get<FVector2D>();
  CHECK_RET(Controller)
  AddMovementInput(GetActorForwardVector(), moveVec.Y);
  AddMovementInput(GetActorRightVector(), moveVec.X);
}

auto AShelterCharacter::look(const FInputActionValue &v) -> void
{
  const auto lookAxisVec = v.Get<FVector2D>();
  CHECK_RET(Controller);
  AddControllerYawInput(lookAxisVec.X);
  AddControllerPitchInput(lookAxisVec.Y);
}

auto AShelterCharacter::getHp() const -> float
{
  return hp;
}

auto AShelterCharacter::applyDamage(float v) -> void
{
  hp -= v;
  if (hp <= 0)
  {
    gameOver();
    return;
  }
  auto hudUi = getHudUi();
  CHECK_RET(hudUi);
  hudUi->setHp(hp);
}

auto AShelterCharacter::getHudUi() -> UShelterHudUi *
{
  auto playerController = Cast<APlayerController>(GetController());
  CHECK_RET(playerController, nullptr);
  auto hud = Cast<AShelterHud>(playerController->GetHUD());
  CHECK_RET(hud, nullptr);
  return hud->getHudUi();
}

auto AShelterCharacter::addScrap() -> void
{
  scrap += rand() % 2 + 1;
  auto hudUi = getHudUi();
  CHECK_RET(hudUi);
  hudUi->setScrap(scrap);
  UGameplayStatics::PlaySoundAtLocation(GetWorld(), scrapSnd, getLoc(this));
}

auto AShelterCharacter::applyShelterDamage(float v) -> void
{
  shelterHp -= v * 0.08f;
  if (shelterHp <= 0)
  {
    gameOver();
    return;
  }
  auto hudUi = getHudUi();
  CHECK_RET(hudUi);
  hudUi->setShelterHp(shelterHp);
}

auto AShelterCharacter::addMedkit() -> void
{
  ++medkits;
  auto hudUi = getHudUi();
  CHECK_RET(hudUi);
  hudUi->setMedkits(medkits);
  UGameplayStatics::PlaySoundAtLocation(GetWorld(), medkitSnd, getLoc(this));
}

auto AShelterCharacter::heal() -> void
{
  if (medkits <= 0)
    return;
  if (hp > 1.f)
    return;
  const auto healAmount = .125f;
  hp += healAmount;
  --medkits;
  auto hudUi = getHudUi();
  CHECK_RET(hudUi);
  hudUi->setHp(hp);
  hudUi->setMedkits(medkits);
  UGameplayStatics::PlaySoundAtLocation(GetWorld(), healingSnd, getLoc(this));
}

auto AShelterCharacter::repair() -> void
{
  if (scrap <= 0)
    return;
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
  if (hitActor->IsA<AShelterShelter>())
  {
    LOG("Facing shelter building");
    if (shelterHp > 1.f)
      return;
    shelterHp += repairAmount();
    --scrap;
    auto hudUi = getHudUi();
    CHECK_RET(hudUi);
    hudUi->setShelterHp(shelterHp);
    hudUi->setScrap(scrap);
    UGameplayStatics::PlaySoundAtLocation(GetWorld(), towerSnd, getLoc(this));
    return;
  }
  if (auto tower = Cast<AShelterTower>(hitActor))
  {
    LOG("Facing tower");
    if (tower->repair())
    {
      --scrap;
      auto hudUi = getHudUi();
      CHECK_RET(hudUi);
      hudUi->setScrap(scrap);
      UGameplayStatics::PlaySoundAtLocation(GetWorld(), towerSnd, getLoc(this));
    }
    return;
  }
  LOG("Facing something else", hitActor->GetName());
}

auto AShelterCharacter::placeTower() -> void
{
  if (scrap <= 0)
    return;

  const auto start = getLoc(this);
  const auto end = start + GetControlRotation().Vector() * 10'00.f;
  FHitResult hitResult;
  auto isHit = GetWorld()->LineTraceSingleByChannel(hitResult, start, end, ECC_Visibility);
  auto l = [&]() {
    if (!isHit)
    {
      LOG("Not pointing at anything");
      return vec(end.X, end.Y, 0.);
    }
    return vec(hitResult.Location.X, hitResult.Location.Y, 0.);
  }();
  FActorSpawnParameters params;
  params.SpawnCollisionHandlingOverride =
    ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
  auto tower =
    GetWorld()->SpawnActor<AShelterTower>(AShelterTower::StaticClass(), l, rot(0., 0., 0.), params);
  if (tower)
  {
    --scrap;
    auto hudUi = getHudUi();
    CHECK_RET(hudUi);
    hudUi->setScrap(scrap);
    UGameplayStatics::PlaySoundAtLocation(GetWorld(), towerSnd, getLoc(this));
  }
}

auto AShelterCharacter::getShelterHp() const -> float
{
  return shelterHp;
}

auto AShelterCharacter::getScrap() const -> int
{
  return scrap;
}

auto AShelterCharacter::gameOver() -> void
{
  UGameplayStatics::OpenLevel(GetWorld(), FName("ShelterGameOver"), true, "0");
}
