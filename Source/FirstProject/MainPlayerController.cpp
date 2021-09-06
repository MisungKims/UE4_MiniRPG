// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"
#include "Blueprint/UserWidget.h"

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (WMainHUD)
	{
		MainHUD = CreateWidget<UUserWidget>(this, WMainHUD);
		if (MainHUD)
		{
			MainHUD->AddToViewport();
			MainHUD->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	if (WEnemyHealthBar)
	{
		EnemyHealthBar = CreateWidget<UUserWidget>(this, WEnemyHealthBar);
		if (EnemyHealthBar)
		{
			EnemyHealthBar->AddToViewport();
			EnemyHealthBar->SetVisibility(ESlateVisibility::Hidden);
		}
		// 정렬
		FVector2D Alignment(0.f, 0.f);
		EnemyHealthBar->SetAlignmentInViewport(Alignment);
	}

	if (WLoadingHUD)
	{
		LoadingHUD = CreateWidget<UUserWidget>(this, WLoadingHUD);
		if (MainHUD)
		{
			LoadingHUD->AddToViewport();
			LoadingHUD->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	if (WPauseMenu)
	{
		PauseMenu = CreateWidget<UUserWidget>(this, WPauseMenu);
		if (PauseMenu)
		{
			PauseMenu->AddToViewport();
			PauseMenu->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	if (WWeaponShop)
	{
		WeaponShop = CreateWidget<UUserWidget>(this, WWeaponShop);
		if (WeaponShop)
		{
			WeaponShop->AddToViewport();
			WeaponShop->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	if (WDieHUD)
	{
		DieHUD = CreateWidget<UUserWidget>(this, WDieHUD);
		if (DieHUD)
		{
			DieHUD->AddToViewport();
			DieHUD->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	if (WStartHUD)
	{
		StartHUD = CreateWidget<UUserWidget>(this, WStartHUD);
		if (StartHUD)
		{
			StartHUD->AddToViewport();
			StartHUD->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void AMainPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (EnemyHealthBar)
	{
		FVector2D PositionInViewport;
		ProjectWorldLocationToScreen(EnemyLocation, PositionInViewport);
		PositionInViewport.Y -= 85.f;

		FVector2D SizeInViewport = FVector2D(300.f, 25.f);

		EnemyHealthBar->SetPositionInViewport(PositionInViewport);
		EnemyHealthBar->SetDesiredSizeInViewport(SizeInViewport);
	}
}

void AMainPlayerController::DisplayEnemyHealthBar()
{
	if (EnemyHealthBar)
	{
		bEnemyHealthBar = true;
		EnemyHealthBar->SetVisibility(ESlateVisibility::Visible);
	}
}

void AMainPlayerController::RemoveEnemyHealthBar()
{
	if (EnemyHealthBar)
	{
		bEnemyHealthBar = false;
		EnemyHealthBar->SetVisibility(ESlateVisibility::Hidden);
	}
}


void AMainPlayerController::DisplayPauseMenu_Implementation()
{
	if (PauseMenu)
	{
		bPauseMenuVisible = true;
		PauseMenu->SetVisibility(ESlateVisibility::Visible);

		// 마우스 커서
		FInputModeGameAndUI InputModeGameAndUI;
		SetInputMode(InputModeGameAndUI);
		bShowMouseCursor = true;
	}
}

void AMainPlayerController::RemovePauseMenu_Implementation()
{
	if (PauseMenu)
	{
		GameModeOnly();
		bShowMouseCursor = false;

		bPauseMenuVisible = false;
	}
}

void AMainPlayerController::TogglePauseMenu()
{
	if (bPauseMenuVisible)
	{
		RemovePauseMenu();
	}
	else
	{
		DisplayPauseMenu();
	}
}

void AMainPlayerController::DisplayMainHUD()
{
	if (MainHUD)
	{
		MainHUD->SetVisibility(ESlateVisibility::Visible);
	}
}

void AMainPlayerController::RemoveMainHUD()
{
	if (MainHUD)
	{
		MainHUD->SetVisibility(ESlateVisibility::Hidden);
	}
}

void AMainPlayerController::DisplayWeaponShop()
{
	if (WeaponShop)
	{
		bWeaponShopVisible = true;
		WeaponShop->SetVisibility(ESlateVisibility::Visible);

		MouseCursorVisible();
	}
}

void AMainPlayerController::RemoveWeaponShop_Implementation()
{
	if (WeaponShop)
	{
		bWeaponShopVisible = false;
		WeaponShop->SetVisibility(ESlateVisibility::Hidden);

		GameModeOnly();
	}
}

void AMainPlayerController::ToggleWeaponShop()
{
	if (bWeaponShopVisible)
	{
		RemoveWeaponShop();
	}
	else
	{
		DisplayWeaponShop();
	}
}

void AMainPlayerController::DisplayLoadingHUD()
{
	if (LoadingHUD)
	{
		LoadingHUD->SetVisibility(ESlateVisibility::Visible);
	}
}

void AMainPlayerController::RemoveLoadingHUD()
{
	if (LoadingHUD)
	{
		LoadingHUD->SetVisibility(ESlateVisibility::Hidden);
	}
}

void AMainPlayerController::DisplayDieHUD()
{
	if (DieHUD)
	{
		DieHUD->SetVisibility(ESlateVisibility::Visible);
	}
}

void AMainPlayerController::RemoveDieHUD_Implementation()
{
	if (DieHUD)
	{
		DieHUD->SetVisibility(ESlateVisibility::Hidden);
	}
}

void AMainPlayerController::DisplayStartHUD()
{
	if (StartHUD)
	{
		StartHUD->SetVisibility(ESlateVisibility::Visible);
	}
}

void AMainPlayerController::RemoveStartHUD()
{
	if (StartHUD)
	{
		StartHUD->SetVisibility(ESlateVisibility::Hidden);
	}
}

void AMainPlayerController::GameModeOnly()
{
	FInputModeGameOnly InputModeGameOnly;
	SetInputMode(InputModeGameOnly);
	bShowMouseCursor = false;
}

void AMainPlayerController::MouseCursorVisible()
{
	// 마우스 커서
	FInputModeGameAndUI InputModeGameAndUI;
	SetInputMode(InputModeGameAndUI);
	bShowMouseCursor = true;
}
