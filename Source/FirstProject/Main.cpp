// Fill out your copyright notice in the Description page of Project Settings.


#include "Main.h"
#include "NPC.h"
#include "Weapon.h"
#include "Enemy.h"
#include "FirstSaveGame.h"
#include "ItemStorage.h"
#include "MainPlayerController.h"
#include "LevelTransitionVolume.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"
#include "Sound/SoundCue.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/PawnMovementComponent.h"

// Sets default values
AMain::AMain()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 600.f;
	CameraBoom->bUsePawnControlRotation = true;

	GetCapsuleComponent()->SetCapsuleSize(25.f, 85.f);

	FollowCamera = CreateAbstractDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
	
	BaseTurnRate = 65.f;
	BaseLookUpRate = 65.f;

	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 650.f;
	GetCharacterMovement()->AirControl = 0.2f;

	DamageCount = 0;

	MaxHP = 100.f;
	HP = 100.f;
	MaxSP = 150.f;
	SP = 150.f;
	Coins = 0;
	Potions = 0;

	RunningSpeed = 650.f;
	SprintingSpeed = 950.f;

	bShiftKeyDown = false;

	MovementStatus = EMovementStatus::EMS_Normal;
	SPStatus = ESPStatus::ESS_Normal;
	HPStatus = EHPStatus::EHS_Normal;

	SPDrainRate = 20.f;
	DecrementSP = 5.f;
	MiddleAttackSP = 50.f;

	MiddleAttackHP = 50.f;

	InterpSpeed = 15.f;
	bInterpToEnemy = false;

	bLMBDown = false;

	bQKeyDown = false;

	bEKeyDown = false;

	bShiftKeyDown = false;

	bAttacking = false;

	bHasCombatTarget = false;

	bMovingForward = false;
	bMovingRight = false;

	bWeaponShop = false;

	bLevelTransitionOnOverlap = false;
}

// Called when the game starts or when spawned
void AMain::BeginPlay()
{
	Super::BeginPlay();
	
	MainPlayerController = Cast<AMainPlayerController>(GetController());

	FString Map = GetWorld()->GetMapName();
	Map.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);

	if (Map == "Start_Map")
	{
		if (MainPlayerController)
		{
			GetMesh()->SetVisibility(false);
			MainPlayerController->MouseCursorVisible();
			MainPlayerController->DisplayStartHUD();
		}
	}
	else
	{
		if (MainPlayerController)
		{
			MainPlayerController->GameModeOnly();
			MainPlayerController->DisplayMainHUD();
		}
	}
	
}

// Called every frame
void AMain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (MovementStatus == EMovementStatus::EMS_Dead) return;

	SetSprint();

	PlayLevelTransitionVolume();

	SetSP(DeltaTime);

	if (bInterpToEnemy && CombatTarget)
	{
		FRotator LookAtYaw = GetLookAtRotationYaw(CombatTarget->GetActorLocation());
		FRotator InterpRotation = FMath::RInterpTo(GetActorRotation(), LookAtYaw, DeltaTime, InterpSpeed);

		SetActorRotation(InterpRotation);
	}

	if (CombatTarget)
	{
		CombatTargetLocation = CombatTarget->GetActorLocation();
		if (MainPlayerController)
		{
			MainPlayerController->EnemyLocation = CombatTargetLocation;
		}
	}
}

FRotator AMain::GetLookAtRotationYaw(FVector Target)
{
	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Target);
	FRotator LookAtRotationYaw(0.f, LookAtRotation.Yaw, 0.f);
	return LookAtRotationYaw;
}

void AMain::SetSprint()
{
	// Sprint
	if (bShiftKeyDown)
	{
		// 움직이고 있을 때만 전력질주 상태로 변경
		if (bMovingForward || bMovingRight)
		{
			SetMovementStatus(EMovementStatus::EMS_Sprinting);
		}
		else
		{
			SetMovementStatus(EMovementStatus::EMS_Normal);
		}
	}
	else
	{
		SetMovementStatus(EMovementStatus::EMS_Normal);
	}
}
void AMain::PlayLevelTransitionVolume()
{
	/** Level Transition Volume */
	if (LevelTransitionVolume)
	{
		if (bFKeyDown)
		{
			if (MainPlayerController)
			{
				MainPlayerController->DisplayLoadingHUD();
			}
			SwitchLevel(LevelTransitionVolume->TransitionLevelName);
			SetLevelTransitionVolume(nullptr);
		}
	}
}
void AMain::SetHP()
{
	/** HP Status */
	switch (HPStatus)
	{
	case EHPStatus::EHS_Normal:
		if (HP <= MiddleAttackHP)
		{
			SetHPStatus(EHPStatus::EHS_BelowMinimum);
		}
		break;

	case EHPStatus::EHS_BelowMinimum:
		if (HP <= 0.f)
		{
			SetHPStatus(EHPStatus::EHS_Exhausted);
		}
		break;

	case  EHPStatus::EHS_Exhausted:

		SetHPStatus(EHPStatus::EHS_ExhaustedRecovering);

		break;

	case EHPStatus::EHS_ExhaustedRecovering:
		if (HP >= MiddleAttackHP)
		{
			SetHPStatus(EHPStatus::EHS_Normal);
		}
		break;
	default:
		;
	}
}
void AMain::SetSP(float DeltaTime)
{
	/** SP Status */
	float DeltaSP = SPDrainRate * DeltaTime;

	switch (SPStatus)
	{
	case ESPStatus::ESS_Normal:
		if (!bLMBDown)
		{
			if (SP + DeltaSP >= MaxSP)
			{
				SP = MaxSP;
			}
			else
			{
				SP += DeltaSP;
			}
		}
		break;

	case ESPStatus::ESS_BelowMinimum:
		if (!bLMBDown)
		{
			if (SP + DeltaSP >= MiddleAttackSP)
			{
				SetSPStatus(ESPStatus::ESS_Normal);
			}
			SP += DeltaSP;
		}
		break;

	case  ESPStatus::ESS_Exhausted:
		if (!bLMBDown)
		{
			SetSPStatus(ESPStatus::ESS_ExhaustedRecovering);
			SP += DeltaSP;
		}
		break;

	case ESPStatus::ESS_ExhaustedRecovering:
		if (SP + DeltaSP >= MiddleAttackSP)
		{
			SetSPStatus(ESPStatus::ESS_Normal);
		}
		SP += DeltaSP;
		break;

	default:
		;
	}
}

// Called to bind functionality to input
void AMain::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMain::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AMain::ShiftKeyDown);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AMain::ShiftKeyUp);

	PlayerInputComponent->BindAction("LMB", IE_Pressed, this, &AMain::LMBDown);
	PlayerInputComponent->BindAction("LMB", IE_Released, this, &AMain::LMBUp);

	PlayerInputComponent->BindAction("NPC", IE_Pressed, this, &AMain::FKeyDown);
	PlayerInputComponent->BindAction("NPC", IE_Released, this, &AMain::FKeyUp);

	PlayerInputComponent->BindAction("Potion", IE_Pressed, this, &AMain::EKeyDown);
	PlayerInputComponent->BindAction("Potion", IE_Released, this, &AMain::EKeyUp);

	PlayerInputComponent->BindAction("Menu", IE_Pressed, this, &AMain::QKeyDown);
	PlayerInputComponent->BindAction("Menu", IE_Released, this, &AMain::QKeyUp);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMain::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMain::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &AMain::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &AMain::LookUp);

}

bool AMain::CanMove(float Value)
{
	FString Map = GetWorld()->GetMapName();
	Map.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);

	if (MainPlayerController)
	{
		return (Value != 0.0f)
			&& (!bAttacking)
			&& (MovementStatus != EMovementStatus::EMS_Dead)
			&& (Map != "Start_Map");
			
	}
	return false;
}

void AMain::MoveForward(float Value)
{
	bMovingForward = false;

	if (CanMove(Value))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);

		bMovingForward = true;
	}
}

void AMain::MoveRight(float value)
{
	bMovingRight = false;
	if (CanMove(value))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, value);

		bMovingRight = true;
	}
}

void AMain::Turn(float Value)
{
	if (CanMove(Value))
	{
		AddControllerYawInput(Value);
	}
}

void AMain::LookUp(float Value)
{
	if (CanMove(Value))
	{
		AddControllerPitchInput(Value);
	}
}


void AMain::LMBDown()
{
	bLMBDown = true;

	if (MovementStatus == EMovementStatus::EMS_Dead) return;

	if (MainPlayerController) if (MainPlayerController->bPauseMenuVisible) return;

	if (EquippedWeapon)
	{
		Attack();
	}
}

void AMain::LMBUp()
{
	bLMBDown = false;
}

void AMain::FKeyDown()
{
	bFKeyDown = true;

	if (NPC)
	{
		if (bFKeyDown && MainPlayerController->WeaponShop)
		{
			MainPlayerController->ToggleWeaponShop();
		}
	}
}

void AMain::FKeyUp()
{
	bFKeyDown = false;
}

void AMain::QKeyDown()
{
	bQKeyDown = true;
	if (MainPlayerController)
	{
		MainPlayerController->TogglePauseMenu();
	}
}
void AMain::QKeyUp()
{
	bQKeyDown = false;
}

void AMain::EKeyDown()
{
	bEKeyDown = true;
	if (Potions > 0)
	{
		IncrementHealth(10);
	}
}

void AMain::EKeyUp()
{
	bEKeyDown = false;
}

void AMain::DecrementHealth(float Amount)
{
	if (HP - Amount <= 0.f)
	{
		HP -= Amount;
		Die();
	}
	else
	{
		HP -= Amount;
	}
}

void AMain::IncrementCoins(int32 Amount)
{
	Coins += Amount;
}

void AMain::DecrementCoins(int32 Amount)
{
	Coins -= Amount;
}

void AMain::IncrementHealth(int32 Amount)
{
	DecrementPotions(1);
	if (HP + Amount >= MaxHP)
	{
		HP = MaxHP;
	}
	else
	{
		HP += Amount;
	}

	if (SP + Amount >= MaxSP)
	{
		SP = MaxSP;
	}
	else
	{
		SP += Amount;
	}
}

void AMain::IncrementPotions(int32 Amount)
{
	Potions += Amount;
}

void AMain::DecrementPotions(int32 Amount)
{
	Potions -= Amount;
}

void AMain::Die()
{
	if (MovementStatus == EMovementStatus::EMS_Dead) return;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && CombatMontage)
	{
		AnimInstance->Montage_Play(CombatMontage, 1.0f);
		AnimInstance->Montage_JumpToSection(FName("Death"));
	}
	SetMovementStatus(EMovementStatus::EMS_Dead);
}

void AMain::Jump()
{
	
	if (MovementStatus != EMovementStatus::EMS_Dead)
	{
		ACharacter::Jump();
	}
}

void AMain::DeathEnd()
{
	GetMesh()->bPauseAnims = true;
	GetMesh()->bNoSkeletonUpdate = true;

	if (MainPlayerController->DieHUD)
	{
		MainPlayerController->MouseCursorVisible();
		MainPlayerController->DisplayDieHUD();
	}
}

void AMain::SetMovementStatus(EMovementStatus Status)
{
	MovementStatus = Status;
	if (MovementStatus == EMovementStatus::EMS_Sprinting)
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintingSpeed;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = RunningSpeed;
	}
}

void AMain::ShiftKeyDown()
{
	bShiftKeyDown = true;
}

void AMain::ShiftKeyUp()
{
	bShiftKeyDown = false;
}

void AMain::ShowPickupLocations()
{
	for (int32 i = 0; i < PickupLocations.Num(); i++)
	{
		UKismetSystemLibrary::DrawDebugSphere(this, PickupLocations[i], 25.f, 8, FLinearColor::Green, 10.f, 2.f);
	}
}

void AMain::SetEquippedWeapon(AWeapon* WeaponToSet)
{
	if (EquippedWeapon)
	{
		EquippedWeapon->Destroy();
	}
	EquippedWeapon = WeaponToSet;
}

void AMain::EquipWeapon()
{
	if (ActiveOverlappingItem)
	{
		AWeapon* Weapon = Cast<AWeapon>(ActiveOverlappingItem);
		if (Weapon)
		{
			Weapon->SkeletalMesh->SetVisibility(true);
			Weapon->Equip(this);
			SetActiveOverlappingItem(nullptr);
		}
	}
}

void AMain::Attack()
{
	if ((!bAttacking) && (MovementStatus != EMovementStatus::EMS_Dead) && (SPStatus != ESPStatus::ESS_Exhausted))
	{
		bAttacking = true;
		SetInerpToEnemy(true);

		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance && CombatMontage)
		{
			int32 Section = FMath::RandRange(0, 1);
			switch (Section)
			{
			case 0:
				AnimInstance->Montage_Play(CombatMontage, 2.2f);
				AnimInstance->Montage_JumpToSection(FName("Attack_1"), CombatMontage);

				break;
			case 1:
				AnimInstance->Montage_Play(CombatMontage, 2.0f);
				AnimInstance->Montage_JumpToSection(FName("Attack_2"), CombatMontage);

				break;
			default:
				break;
			}
		}
	}

	/** SP Status */
	if (bAttacking)
	{
		switch (SPStatus)
		{
		case ESPStatus::ESS_Normal:

			if (SP - DecrementSP <= MiddleAttackSP)
			{
				SetSPStatus(ESPStatus::ESS_BelowMinimum);
			}
			SP -= DecrementSP;
			break;

		case ESPStatus::ESS_BelowMinimum:

			if (SP - DecrementSP <= 0.f)
			{
				SetSPStatus(ESPStatus::ESS_Exhausted);
				SP = 0.f;
			}
			else
			{
				SP -= DecrementSP;
			}
			break;

		case  ESPStatus::ESS_Exhausted:
			SP = 0.f;
			break;

		case ESPStatus::ESS_ExhaustedRecovering:
			if (SP - DecrementSP <= 0.f)
			{
				SetSPStatus(ESPStatus::ESS_Exhausted);
				SP = 0.f;
			}
			else
			{
				SP -= DecrementSP;
			}
			break;

		default:
			;
		}
	}
}

void AMain::AttackEnd()
{
	bAttacking = false;
	SetInerpToEnemy(false);
	if (bLMBDown)
	{
		Attack();
	}
}

void AMain::PlaySwingSound()
{
	if (EquippedWeapon->SwingSound)
	{
		UGameplayStatics::PlaySound2D(this, EquippedWeapon->SwingSound);
	}
}

void AMain::SetInerpToEnemy(bool Interp)
{
	bInterpToEnemy = Interp;
}

float AMain::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	if (!bHasCombatTarget)
	{
		DamageCount = 0;
	}

	if (HP - DamageAmount <= 0.f)
	{
		HP -= DamageAmount;
		Die();
		if (DamageCauser)
		{
			AEnemy* Enemy = Cast<AEnemy>(DamageCauser);
			if (Enemy)
			{
				Enemy->bHasValidTarget = false;
			}
		}
	}
	else
	{
		DamageCount++;
		if (DamageCount == 5)
		{
			UE_LOG(LogTemp, Log, TEXT("Main Damage Count = 5"));
			HP -= DamageAmount;	//콤보 발동 시 기존 데미지의 2배 입힘
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			if (AnimInstance && CombatMontage)
			{
				AnimInstance->Montage_Play(CombatMontage, 1.0f);
				AnimInstance->Montage_JumpToSection(FName("Hurt"));
			}
			DamageCount = 0;
		}
		HP -= DamageAmount;
	}

	return DamageAmount;
}

void AMain::UpdateCombatTarget()
{
	TArray<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors, EnemyFilter);

	if (OverlappingActors.Num() == 0)
	{
		if (MainPlayerController)
		{
			MainPlayerController->RemoveEnemyHealthBar();
		}
		return;
	}

	AEnemy* ClosestEnemy = Cast<AEnemy>(OverlappingActors[0]);
	if (ClosestEnemy)
	{
		FVector Location = GetActorLocation();
		// 가장 가까운 적과 main 사이의 거리
		float MinDistance = (ClosestEnemy->GetActorLocation() - Location).Size();

		for (auto Actor : OverlappingActors)
		{
			AEnemy* Enemy = Cast<AEnemy>(Actor);
			if (Enemy)
			{
				float DistanceToActor = (Enemy->GetActorLocation() - Location).Size();
				// 제일 가까운 적을 대상으로
				if (DistanceToActor < MinDistance)
				{
					MinDistance = DistanceToActor;
					ClosestEnemy = Enemy;
				}
			}			
		}
		if (MainPlayerController)
		{
			MainPlayerController->DisplayEnemyHealthBar();
		}
		SetCombatTarget(ClosestEnemy);
		bHasCombatTarget = true;
	}
}


void AMain::SwitchLevel(FName LevelName)
{
	UWorld* World = GetWorld();
	if (World)
	{
		FString CurrentLevel = World->GetMapName();
		FName CurrentLevelName(CurrentLevel);
		if (CurrentLevelName != LevelName)
		{
		
			if (LevelName != "Start_Map")
			{
				if (MainPlayerController)
				{
					GetMesh()->SetVisibility(true);
					MainPlayerController->GameModeOnly();
					MainPlayerController->DisplayMainHUD();
					MainPlayerController->RemoveLoadingHUD();
					MainPlayerController->RemoveStartHUD();
				}
			}
			else
			{
				if (MainPlayerController)
				{
					GetMesh()->SetVisibility(false);
					MainPlayerController->MouseCursorVisible();
					MainPlayerController->DisplayStartHUD();
					MainPlayerController->RemoveMainHUD();
					MainPlayerController->RemoveLoadingHUD();
				}
			}
			if (TransitionSound)
			{
				UGameplayStatics::PlaySound2D(this, TransitionSound);
			}

			if (WeaponStorage)
			{
				AItemStorage* Weapons = GetWorld()->SpawnActor<AItemStorage>(WeaponStorage);
				if (Weapons)
				{
					FString WeaponName = TEXT("Glimmer");
					
					if (Weapons->WeaponMap.Contains(WeaponName))
					{
						AWeapon* WeaponToEquip = GetWorld()->SpawnActor<AWeapon>(Weapons->WeaponMap[WeaponName]);
						WeaponToEquip->Equip(this);
					}
				}
			}
			UGameplayStatics::OpenLevel(World, LevelName);

		}
	}
}

void AMain::NewGame()
{
	HP = 100;
	MaxHP = 100;
	SP = 100;
	MaxSP = 100;
	Coins = 0;
	Potions = 0;

	UFirstSaveGame* SaveGameInstance = Cast<UFirstSaveGame>(UGameplayStatics::CreateSaveGameObject(UFirstSaveGame::StaticClass()));

	SaveGameInstance->CharacterStats.HP = HP;
	SaveGameInstance->CharacterStats.MaxHP = MaxHP;
	SaveGameInstance->CharacterStats.SP = SP;
	SaveGameInstance->CharacterStats.MaxSP = MaxSP;
	SaveGameInstance->CharacterStats.Coins = Coins;
	SaveGameInstance->CharacterStats.Potions = Potions;

	SetMovementStatus(EMovementStatus::EMS_Normal);
	GetMesh()->bPauseAnims = false;
	GetMesh()->bNoSkeletonUpdate = false;

	SaveGameInstance->CharacterStats.LevelName = "MainVillage_Map";

	UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveGameInstance->PlayerName, SaveGameInstance->UserIndex);

	SwitchLevel("MainVillage_Map");
}


void AMain::SaveGame()
{
	// 게임 개체를 함수로 저장
	UFirstSaveGame* SaveGameInstance = Cast<UFirstSaveGame>(UGameplayStatics::CreateSaveGameObject(UFirstSaveGame::StaticClass()));

	SaveGameInstance->CharacterStats.HP = HP;
	SaveGameInstance->CharacterStats.MaxHP = MaxHP;
	SaveGameInstance->CharacterStats.SP = SP;
	SaveGameInstance->CharacterStats.MaxSP = MaxSP;
	SaveGameInstance->CharacterStats.Coins = Coins;
	SaveGameInstance->CharacterStats.Potions = Potions;

	FString MapName = GetWorld()->GetMapName();
	MapName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);

	SaveGameInstance->CharacterStats.LevelName = MapName;

	if (EquippedWeapon)
	{
		SaveGameInstance->CharacterStats.WeaponName = EquippedWeapon->Name;
	}

	SaveGameInstance->CharacterStats.Location = GetActorLocation();
	SaveGameInstance->CharacterStats.Rotation = GetActorRotation();

	UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveGameInstance->PlayerName, SaveGameInstance->UserIndex);
}


void AMain::LoadGame(bool SetPosition)
{
	UFirstSaveGame* LoadGameInstance = Cast<UFirstSaveGame>(UGameplayStatics::CreateSaveGameObject(UFirstSaveGame::StaticClass()));

	LoadGameInstance = Cast<UFirstSaveGame>(UGameplayStatics::LoadGameFromSlot(LoadGameInstance->PlayerName, LoadGameInstance->UserIndex));

	HP = LoadGameInstance->CharacterStats.HP;
	MaxHP = LoadGameInstance->CharacterStats.MaxHP;
	SP = LoadGameInstance->CharacterStats.SP;
	MaxSP = LoadGameInstance->CharacterStats.MaxSP;
	Coins = LoadGameInstance->CharacterStats.Coins;
	Potions = LoadGameInstance->CharacterStats.Potions;

	if (WeaponStorage)
	{
		AItemStorage* Weapons = GetWorld()->SpawnActor<AItemStorage>(WeaponStorage);
		if (Weapons)
		{
			FString WeaponName = LoadGameInstance->CharacterStats.WeaponName;

			if (Weapons->WeaponMap.Contains(WeaponName))
			{
				AWeapon* WeaponToEquip = GetWorld()->SpawnActor<AWeapon>(Weapons->WeaponMap[WeaponName]);
				WeaponToEquip->Equip(this);
			}
		}
	}
	
	if (SetPosition)
	{
		SetActorLocation(LoadGameInstance->CharacterStats.Location);
		SetActorRotation(LoadGameInstance->CharacterStats.Rotation);
	}

	SetMovementStatus(EMovementStatus::EMS_Normal);
	GetMesh()->bPauseAnims = false;
	GetMesh()->bNoSkeletonUpdate = false;

	if (LoadGameInstance->CharacterStats.LevelName != TEXT(""))
	{
		FName LevelName(LoadGameInstance->CharacterStats.LevelName);

		SwitchLevel(LevelName);
	}
}