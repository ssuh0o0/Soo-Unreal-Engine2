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

	InitialDirection = FVector(0.0f, 0.0f, 0.0f);

	bInitializeFloaterLocation = false;
	bShouldFloat = false;

	InitialForce = FVector(20000.f,0.0f,0.0f);
	InitialTorque = FVector(20000.f,0.0f,0.0f);

	RunningTime = 0.0f;

	A = 0.f;
	B = 0.f;
	C = 0.f;
	D = 0.f;
}

// Called when the game starts or when spawned
void AFloater::BeginPlay()
{
	Super::BeginPlay();

	//float InitialX = FMath::FRand();
	float InitialX = FMath::FRandRange(-500.f,500.f);
	float InitialY = FMath::FRandRange(-500.f,500.f);
	float InitialZ = FMath::FRandRange(0.f,500.f);

	InitialLocation.X = InitialX;
	InitialLocation.Y = InitialY;
	InitialLocation.Z = InitialZ;

	//InitialLocation *= 500.0f;

	PlacedLocation = GetActorLocation();
	
	if (bInitializeFloaterLocation){
	SetActorLocation(InitialLocation);
	}
	FHitResult HitResult;
	BaseZLocation = PlacedLocation.Z;
	//*** AddActorLocalOffset 액터를 기준으로 x,y,z 축으로 이동하게 함. ***
	//FVector LocalOffset = FVector(20.0f,0.0f,0.0f);
	//AddActorLocalOffset(InitialDirection,true,&HitResult);

	//*** AddActorWorldOffset 월드를 기준으로 x,y,z 축으로 이동하게 함. ***
	//FVector WorldOffset = FVector(20.0f,0.0f,0.0f);
	//AddActorWorldOffset(InitialDirection,true,&HitResult);

	//FRotator Rotation = FRotator(0.0f,0.0f,0.0f);
	//*** AddActorLocalRotation 액터를 기준으로 돌아가게 함. ***
	//AddActorLocalRotation(Rotation);
	//*** AddActorWorldRotation 월드를 기준으로 돌아가게 함. ***
	//AddActorWorldRotation(Rotation);

	//StaticMesh-> AddForce(InitialForce);
	//StaticMesh-> AddTorque(InitialTorque);

}

// Called every frame
void AFloater::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bShouldFloat){

		// FHitResult HitResult;
		// AddActorLocalOffset(InitialDirection,true,&HitResult);
		// FVector HitLocation = HitResult.Location;
		// UE_LOG(LogTemp, Warning, TEXT("Hit Location X = %f , Y = %f, Z = %f "),HitLocation.X,HitLocation.Y,HitLocation.Z);
	
		//sin, cos 이용해서 진동만들기
		FVector NewLocation = GetActorLocation();
		NewLocation.Z = BaseZLocation + A *FMath::Sin(B * RunningTime - C) + D;  //Period = 2 * PI / ABS(B)
		SetActorLocation(NewLocation);
		RunningTime += DeltaTime;

	}
}

