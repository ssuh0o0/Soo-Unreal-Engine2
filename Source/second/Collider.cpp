// Fill out your copyright notice in the Description page of Project Settings.


#include "Collider.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "ColliderPawnMovement.h"

// Sets default values
ACollider::ACollider()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	// SphereComponent->SetupAttachment(GetRootComponent());
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SetRootComponent(SphereComponent);
	SphereComponent-> InitSphereRadius(40.f);
	SphereComponent-> SetCollisionProfileName(TEXT("Pawn"));

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	MeshComponent->SetupAttachment(GetRootComponent());
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshComponentAsset(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));
	if(MeshComponentAsset.Succeeded()){
		MeshComponent->SetStaticMesh(MeshComponentAsset.Object);
		MeshComponent->SetRelativeLocation(FVector(0.f,0.f,-40.f));
		MeshComponent->SetWorldScale3D(FVector(0.8f,0.8f,0.8f));
	}

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->SetRelativeRotation(FRotator(-45.f,0.f,0.f)) ;
	SpringArm->TargetArmLength = 400.f;
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 3.f;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera -> SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	OurMovementComponent = CreateDefaultSubobject<UColliderPawnMovement>(TEXT("OurMovementComponent"));
	OurMovementComponent -> UpdatedComponent = RootComponent; 
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	CameraInput = FVector2D(0.f,0.f);
}

// Called when the game starts or when spawned
void ACollider::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACollider::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FRotator NewRotation = GetActorRotation();
	NewRotation.Yaw += CameraInput.X;
	SetActorRotation(NewRotation);

	FRotator NewSpringArmRotation = SpringArm -> GetComponentRotation();
	//pitch가 -15.f 보다 높게 올라가면 별로 안좋음 -> 근데 해봤더니 -15.f로 하면 하늘이 안보임. 그래서 0.f 로 올려줌
	NewSpringArmRotation.Pitch = FMath::Clamp(NewSpringArmRotation.Pitch += CameraInput.Y,-80.f,-0.f);

	SpringArm -> SetWorldRotation(NewSpringArmRotation);
}

// Called to bind functionality to input
void ACollider::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent){
	
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("MoveForward"),this, &ACollider::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"),this, &ACollider::MoveRight);

	PlayerInputComponent->BindAxis(TEXT("CameraPitch"),this, &ACollider::CameraPitch);
	PlayerInputComponent->BindAxis(TEXT("CameraYaw"),this, &ACollider::CameraYaw);
}

void ACollider::MoveForward(float value){

	FVector Forward = GetActorForwardVector();
	//AddMovementInput(value * Forward);
	if (OurMovementComponent)
	{
		OurMovementComponent->AddInputVector(value * Forward);
	}
}

void ACollider::MoveRight(float value){
	
	FVector Right = GetActorRightVector();
	//AddMovementInput(value * Right);
	if (OurMovementComponent)
	{
		OurMovementComponent->AddInputVector(value * Right);
	}
}

void ACollider::CameraPitch(float AxisValue){
	CameraInput.Y = AxisValue;
}

void ACollider::CameraYaw(float AxisValue){
	CameraInput.X = AxisValue;
}

UPawnMovementComponent* ACollider::GetMovementComponent() const {
	return OurMovementComponent;
}
