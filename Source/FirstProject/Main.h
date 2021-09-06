// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Main.generated.h"

/** Movement ENUM */
UENUM(BlueprintType)
enum class EMovementStatus : uint8
{
	EMS_Normal UMETA(DisplayName = "Normal"),
	EMS_Sprinting UMETA(DisplayName = "Sprinting"), 
	EMS_Dead UMETA(DisplayName = "Dead"),

	EMS_MAS UMETA(DisplayName = "DefaultMax")
};

/** SP Status ENUM */
UENUM(BlueprintType)
enum class ESPStatus : uint8
{
	ESS_Normal UMETA(DisplayName = "Normal"),
	ESS_BelowMinimum UMETA(DisplayName = "BelowMinimum"),

	ESS_Exhausted UMETA(DisplayName = "Exhausted"),							// 지친 상태
	ESS_ExhaustedRecovering UMETA(DisplayName = "ExhaustedRecovering"),		// 지친 상태에서 회복

	ESS_MAX UMETA(DisplayName = "DefaultMax")
};

/** HP Status ENUM */
UENUM(BlueprintType)
enum class EHPStatus : uint8
{
	EHS_Normal UMETA(DisplayName = "Normal"),
	EHS_BelowMinimum UMETA(DisplayName = "BelowMinimum"),

	EHS_Exhausted UMETA(DisplayName = "Exhausted"),							// 지친 상태
	EHS_ExhaustedRecovering UMETA(DisplayName = "ExhaustedRecovering"),		// 지친 상태에서 회복

	EHS_MAX UMETA(DisplayName = "DefaultMax")
};



UCLASS()
class FIRSTPROJECT_API AMain : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMain();

	void SetSprint();
	void PlayLevelTransitionVolume();
	void SetHP();
	void SetSP(float DeltaTime);

	/** Camera */

	/** Camera boom positioning the camera behind the player */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	/** Follow Camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;

	/** Base turn rate to scale turning functions for the camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;

	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }


	/** MainPlayerController*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Controller")
		class AMainPlayerController* MainPlayerController;


	/** Item */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = items)
		class AItem* ActiveOverlappingItem;

	FORCEINLINE void SetActiveOverlappingItem(AItem* item) { ActiveOverlappingItem = item; }


	/** Weapon */
	UPROPERTY(EditDefaultsOnly, Category = "SaveData")
	TSubclassOf<class AItemStorage> WeaponStorage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = items)
		class AWeapon* EquippedWeapon;

	FORCEINLINE void SetEquippedWeapon(AWeapon* weaponToSet);
	FORCEINLINE AWeapon* GetEquippedWeapon() { return EquippedWeapon; }
	
	UFUNCTION(BlueprintCallable)
		void EquipWeapon();


	/** Combat */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	bool bHasCombatTarget;

	FORCEINLINE void SetHasCombatTarget(bool HasTarget) { bHasCombatTarget = HasTarget; }

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
	FVector CombatTargetLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	class UParticleSystem* HitParticles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	class USoundCue* HitSound;

	// enemy를 자동으로 회전시킬 때 필요
	float InterpSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
		bool bInterpToEnemy;

	void SetInerpToEnemy(bool Interp);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
		class AEnemy* CombatTarget;

	FORCEINLINE void SetCombatTarget(AEnemy* Target) { CombatTarget = Target; }

	FRotator GetLookAtRotationYaw(FVector Target);

	void UpdateCombatTarget();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combat")
		TSubclassOf<AEnemy> EnemyFilter;

	int32 DamageCount;


	/** Pick up Location */
	TArray<FVector> PickupLocations;

	UFUNCTION(BlueprintCallable)
	void ShowPickupLocations();


	/** Movement */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enums")
	EMovementStatus MovementStatus;

	void SetMovementStatus(EMovementStatus Status);


	/** HP */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enums")
		EHPStatus HPStatus;

	FORCEINLINE void SetHPStatus(EHPStatus Status) { HPStatus = Status; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HP")
		float MiddleAttackHP;


	/** SP */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enums")
		ESPStatus SPStatus;

	FORCEINLINE void SetSPStatus(ESPStatus Status) { SPStatus = Status; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SP")
		float MiddleAttackSP;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SP")
		float DecrementSP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		float SPDrainRate;


	/** Level Transition Volume */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level")
		class ALevelTransitionVolume* LevelTransitionVolume;

	FORCEINLINE void SetLevelTransitionVolume(ALevelTransitionVolume* level) { LevelTransitionVolume = level; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transition")
		class USoundCue* TransitionSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Transition")
		bool bLevelTransitionOnOverlap;

	UFUNCTION(BlueprintCallable)
	void SwitchLevel(FName LevelName);

	/** Increment Coins & Potions */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transition")
		class USoundCue* GetSound;

	/**
	/*
	/* Player Stats
	/*
	*/

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Stats")
		float MaxHP;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Stats")
		float HP;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Stats")
		float MaxSP;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Stats")
		float SP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
		int32 Coins;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
		int32 Potions;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	void Die();

	virtual void Jump() override;

	void DecrementHealth(float Amount);

	UFUNCTION(BlueprintCallable)
	void IncrementCoins(int32 Amount);

	UFUNCTION(BlueprintCallable)
		void DecrementCoins(int32 Amount);

	UFUNCTION(BlueprintCallable)
	void IncrementHealth(int32 Amount);
	
	UFUNCTION(BlueprintCallable)
		void IncrementPotions(int32 Amount);

	UFUNCTION(BlueprintCallable)
		void DecrementPotions(int32 Amount);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	/** Main Movement */
	void MoveForward(float Value);

	bool bMovingForward;

	void MoveRight(float value);

	bool bMovingRight;

	void Turn(float Value);

	void LookUp(float Value);

	bool CanMove(float value);

	void ShiftKeyDown();

	void ShiftKeyUp();

	bool bShiftKeyDown;

	void FKeyDown();

	void FKeyUp();

	bool bFKeyDown;

	void LMBDown();

	void LMBUp();

	bool bLMBDown;

	void QKeyDown();

	void QKeyUp();

	bool bQKeyDown;

	void EKeyDown();

	void EKeyUp();

	bool bEKeyDown;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Running")
		float RunningSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Running")
		float SprintingSpeed;


	/** Animation */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Anims")
	bool bAttacking;

	void Attack();

	UFUNCTION(BlueprintCallable)
	void AttackEnd();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anims")
	class UAnimMontage* CombatMontage;

	UFUNCTION(BlueprintCallable)
	void PlaySwingSound();

	UFUNCTION(BlueprintCallable)
	void DeathEnd();

	


	/** Save Game */
	UFUNCTION(BlueprintCallable)
	void SaveGame();


	/** Load Game */
	UFUNCTION(BlueprintCallable)
	void LoadGame(bool SetPosition);


	UFUNCTION(BlueprintCallable)
		void NewGame();

	/** NPC */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NPC")
		class ANPC* NPC;

	FORCEINLINE void SetNPC(ANPC* closeNPC) { NPC = closeNPC; }

	bool bWeaponShop;

};
