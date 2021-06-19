// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Components/SphereComponent.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AgroSphere = CreateDefaultSubobject<USphereComponent>((TEXT("AgroSphere")));
	AgroSphere -> SetupAttachment(GetRootComponent());
	AgroSphere -> InitSphereRadius(500.f);

	CombatSphere = CreateDefaultSubobject<USphereComponent>((TEXT("CombatSphere")));
	CombatSphere -> SetupAttachment(GetRootComponent());
	CombatSphere -> InitSphereRadius(75.f);
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

