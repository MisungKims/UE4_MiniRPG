// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC.h"
#include "Main.h"
#include "MainPlayerController.h"

#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/TextRenderComponent.h"

// Sets default values
ANPC::ANPC()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AgroSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AgroSphere"));
	AgroSphere->SetupAttachment(GetRootComponent());
	AgroSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Ignore);
	AgroSphere->InitSphereRadius(300.f);

	FString NPCtxt = TEXT("Press F Key");

	TextRenderComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TextRenderComponent"));
	TextRenderComponent->SetupAttachment(GetRootComponent());
	TextRenderComponent->SetText(NPCtxt);
	TextRenderComponent->SetVisibility(false);
	TextRenderComponent->SetHorizontalAlignment(EHTA_Center);
	TextRenderComponent->SetVerticalAlignment(EVRTA_TextCenter);

}

// Called when the game starts or when spawned
void ANPC::BeginPlay()
{
	Super::BeginPlay();
	
	AgroSphere->OnComponentBeginOverlap.AddDynamic(this, &ANPC::AgroSphereOnOverlapBegin);
	AgroSphere->OnComponentEndOverlap.AddDynamic(this, &ANPC::AgroSphereOnOverlapEnd);

	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
}

// Called every frame
void ANPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ANPC::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ANPC::AgroSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		AMain* Main = Cast<AMain>(OtherActor);
		if (Main)
		{
			if (Main->MainPlayerController)
			{
				Main->SetNPC(this);
				TextRenderComponent->SetVisibility(true);
			}
		}
	}
}

void ANPC::AgroSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		AMain* Main = Cast<AMain>(OtherActor);
		if (Main)
		{
			if (Main->MainPlayerController)
			{
				Main->SetNPC(nullptr);
				TextRenderComponent->SetVisibility(false);
				if (Main->MainPlayerController->bWeaponShopVisible)
				{
					Main->MainPlayerController->RemoveWeaponShop();
				}
			}
		}
	}
}