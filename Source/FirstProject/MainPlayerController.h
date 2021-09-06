// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class FIRSTPROJECT_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:

	/** Main Widget */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		TSubclassOf<class UUserWidget> WMainHUD;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Widgets")
		UUserWidget* MainHUD;

	void DisplayMainHUD();

	void RemoveMainHUD();


	/** Loading Widget */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		TSubclassOf<class UUserWidget> WLoadingHUD;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Widgets")
		UUserWidget* LoadingHUD;

	void DisplayLoadingHUD();

	void RemoveLoadingHUD();


	/** Enmemy Health Bar */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<UUserWidget>  WEnemyHealthBar;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Widgets")
	UUserWidget* EnemyHealthBar;

	bool bEnemyHealthBar;

	void DisplayEnemyHealthBar();
	void RemoveEnemyHealthBar();

	FVector EnemyLocation;


	/** Pause Menu */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		TSubclassOf<UUserWidget>  WPauseMenu;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Widgets")
		UUserWidget* PauseMenu;

	bool bPauseMenuVisible;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD")
	void DisplayPauseMenu();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD")
	void RemovePauseMenu();

	void TogglePauseMenu();


	/** Weapon Shop */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		TSubclassOf<class UUserWidget> WWeaponShop;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Widgets")
		UUserWidget* WeaponShop;

	void DisplayWeaponShop();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD")
		void RemoveWeaponShop();

	void ToggleWeaponShop();

	bool bWeaponShopVisible;


	/** Die Widget */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		TSubclassOf<class UUserWidget> WDieHUD;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Widgets")
		UUserWidget* DieHUD;

	void DisplayDieHUD();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD")
	void RemoveDieHUD();

	/** Start Widget */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		TSubclassOf<class UUserWidget> WStartHUD;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Widgets")
		UUserWidget* StartHUD;

	void DisplayStartHUD();

	void RemoveStartHUD();


	/** Cursor */
	void MouseCursorVisible();

	void GameModeOnly();

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;
};
