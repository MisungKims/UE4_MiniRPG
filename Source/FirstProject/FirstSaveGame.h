// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "FirstSaveGame.generated.h"

USTRUCT(BlueprintType)
struct FCharacterStats
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	float HP;

	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	float MaxHP;

	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	float SP;

	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	float MaxSP;

	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	int32 Coins;

	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	int32 Potions;

	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	FVector Location;

	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	FRotator Rotation;

	UPROPERTY(EditDefaultsOnly, Category = "SaveData")
	FString WeaponName;

	UPROPERTY(EditDefaultsOnly, Category = "SaveData")
		FString LevelName;
};

/**
 * 
 */
UCLASS()
class FIRSTPROJECT_API UFirstSaveGame : public USaveGame
{
	GENERATED_BODY()

public:

	UFirstSaveGame();

	UPROPERTY(VisibleAnywhere, Category = Basic)
	FCharacterStats CharacterStats;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	FString PlayerName;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	uint32 UserIndex;
};
