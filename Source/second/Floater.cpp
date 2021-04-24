// Fill out your copyright notice in the Description page of Project Settings.


#include "Floater.h"

// Sets default values
AFloater::AFloater()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CustomStaticMesh"));

	InitialLocation = FVector(0.0f);
	PlacedLocation = FVector(0.0f);
	WorldOrigin = FVector(0.0f, 0.0f, 0.0f);

}

// Called when the game starts or when spawned
void AFloater::BeginPlay()
{
	Super::BeginPlay();

	PlacedLocation = GetActorLocation();
	
	if (bInitializeFloaterLocation){
	SetActorLocation(InitialLocation);
	}
	FHitResult HitResult;

	//AddActorLocalOffset 액터를 기준으로 x,y,z 축으로 이동하게 함.
	FVector LocalOffset = FVector(20.0f,0.0f,0.0f);
	AddActorLocalOffset(InitialDirection,true,&HitResult);

	//AddActorWorldOffset 월드를 기준으로 x,y,z 축으로 이동하게 함.
	FVector WorldOffset = FVector(20.0f,0.0f,0.0f);
	AddActorWorldOffset(InitialDirection,true,&HitResult);

	FRotator Rotation = FRotator(0.0f,0.0f,0.0f);
	//AddActorLocalRotation 액터를 기준으로 돌아가게 함.
	AddActorLocalRotation(Rotation);
	//AddActorWorldRotation 월드를 기준으로 돌아가게 함.
	AddActorWorldRotation(Rotation);

}

// Called every frame
void AFloater::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bShouldFloat){

		FHitResult HitResult;
		AddActorLocalOffset(InitialDirection,true,&HitResult);

		FVector HitLocation = HitResult.Location;
		UE_LOG(LogTemp, Warning, TEXT("Hit Location X = %f , Y = %f, Z = %f "),HitLocation.X,HitLocation.Y,HitLocation.Z);
	}
}

