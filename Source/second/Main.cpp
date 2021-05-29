// Fill out your copyright notice in the Description page of Project Settings.


#include "Main.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Engine/World.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

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

	// 충돌 캡슐을 위해 사이즈 조정
	GetCapsuleComponent()->SetCapsuleSize(48.f, 105.f);

	// Create Follow Camera 
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera -> SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera -> bUsePawnControlRotation = false; // 카메라는 Rotation에 구애를 받으면 안됨.

	//Set our turn rates for input
	BaseTurnRate = 65.f;
	BaseLookUpRate = 65.f;

	// 컨트롤러가 rotate할 때, rotate 하지 않기
	// 카메라에만 영향 미치도록
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	// 캐릭터 움직임을 변경
	GetCharacterMovement() -> bOrientRotationToMovement = true; //캐릭터가 입력방향으로 움직임.
	GetCharacterMovement() -> RotationRate = FRotator(0.0f , 540.f,0.0f); // 회전 방향
	GetCharacterMovement() -> JumpZVelocity = 650.f;
	GetCharacterMovement() -> AirControl = 0.2f;


	MaxHealth = 100.f ;
	Health = 65.f ;

	MaxStamina = 350.f;
	Stamina = 120.f;

	Coins = 0; 
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
	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMain::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMain::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AMain::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AMain::LookUpAtRate);
}

void AMain::MoveForward(float value){

	if ((Controller != nullptr) && (value != 0.f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller -> GetControlRotation();
		const FRotator YawRotation(0.f,Rotation.Yaw,0.f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction,value);
	}
}

void AMain::MoveRight(float value){
	
	if ((Controller != nullptr) && (value != 0.f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller -> GetControlRotation();
		const FRotator YawRotation(0.f,Rotation.Yaw,0.f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction,value);
	}
}

void AMain::TurnAtRate(float Rate){
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}


void AMain::LookUpAtRate(float Rate){
	AddControllerYawInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AMain::DecrementHealth(float Amount)
{
	Health -= Amount;
	if (Health - Amount <= 0.f)
	{	
		Die();
	}
}

void AMain::IncrementCoin(int32 Amount)
{
	Coins += Amount;
}

void AMain::Die()
{

}
