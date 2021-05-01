// Fill out your copyright notice in the Description page of Project Settings.


#include "Main.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
AMain::AMain()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Create Camera Boom (pulls towards the player if there's a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom -> SetupAttachment(GetRootComponent());
	CameraBoom -> TargetArmLength = 600.f; //Camera follows at this distance
	CameraBoom -> bUsePawnControlRotation = true; //Rotate arm based on controller

	// Create Follow Camera 
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera -> SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera -> bUsePawnControlRotation = false; // 카메라는 Rotation에 구애를 받으면 안됨.

	//Set our turn rates for input
	BaseTurnRate = 65.f;
	BaseLookRate = 65.f;
}

// Called when the game starts or when spawned
void AMain::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMain::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMain::MoveForward(float value){

	FVector Forward = GetActorForwardVector();
	//AddMovementInput(value * Forward);
	if (OurMovementComponent)
	{
		OurMovementComponent->AddInputVector(value * Forward);
	}
}

void AMain::MoveRight(float value){
	
	FVector Right = GetActorRightVector();
	//AddMovementInput(value * Right);
	if (OurMovementComponent)
	{
		OurMovementComponent->AddInputVector(value * Right);
	}
}
