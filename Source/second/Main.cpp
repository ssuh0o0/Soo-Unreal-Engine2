// Fill out your copyright notice in the Description page of Project Settings.


#include "Main.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Engine/World.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Weapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"

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

	MaxStamina = 150.f;
	Stamina = 120.f;

	Coins = 0; 

	RunningSpeed = 650.f ;
	SprintingSpeed = 950.f;
	bShiftKeyDown = false ;

	bLMBDown = false;

	//Initialize Enums
	MovementStatus = EMovementStatus::EMS_Normal;
	StaminaStatus = EStaminaStatus::ESS_Normal;

	StaminaDrainRate = 25.f ;
	MinSprintStamina = 50.f ;

 	InterpSpeed = 15.f;
	bInterpToEnemy = false;
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

	// 체력이 특정 프레임에서 얼마나 변해야 하는지
	float DeltaStamina = StaminaDrainRate * DeltaTime;

	switch (StaminaStatus)
	{
		case EStaminaStatus::ESS_Normal:	

			if (bShiftKeyDown) //shift 키가 눌려있다면
			{	// MinSprintStatmina (스프린트상태의 최소 보다 작다면)
				if (Stamina - DeltaStamina <= MinSprintStamina)
				{	//BelowMin 상태로 바꿈
					SetStaminaStatus(EStaminaStatus::ESS_BelowMin);
					Stamina -= DeltaStamina;
				}
				else //상태는 바뀌지 않았지만, stamina는 계속 감소 
				{
					Stamina -= DeltaStamina;
				}
				SetMovementStatus(EMovementStatus::EMS_Sprinting);
			}
			else
			{
				if (Stamina + DeltaStamina >= MaxStamina)
				{
					Stamina = MaxStamina;
				}
				else
				{
					Stamina += DeltaStamina ;
				}
				SetMovementStatus(EMovementStatus::EMS_Normal);
			}

			break;
		case EStaminaStatus::ESS_BelowMin:

			if (bShiftKeyDown)
			{
				if (Stamina - DeltaStamina <= 0.f) //지친 상태
				{
					SetStaminaStatus(EStaminaStatus::ESS_Exhausted);
					Stamina = 0.f ;
					SetMovementStatus(EMovementStatus::EMS_Normal);
				}
				else //지치지 않고 달리지만, Min보다 낮은상태 (색깔만 바뀌게 됨)
				{
					Stamina -= DeltaStamina;
					SetMovementStatus(EMovementStatus::EMS_Sprinting);
				}
			}
			else
			{
				if (Stamina + DeltaStamina >= MinSprintStamina)
				{
					SetStaminaStatus(EStaminaStatus::ESS_Normal);
					Stamina += DeltaStamina ;
				}
				else
				{
					Stamina += DeltaStamina ;
				}
				SetMovementStatus(EMovementStatus::EMS_Normal);
			}

			break;
		case EStaminaStatus::ESS_Exhausted:
			if (bShiftKeyDown)
			{
				Stamina = 0.f ;
			}
			else
			{	
				//체력이 소진된 후, shift키가 눌려있지 않다면, 체력 회복중 상태로 바뀜
				SetStaminaStatus(EStaminaStatus::ESS_ExhaustedRecovering);
				Stamina += DeltaStamina ;
			}
			SetMovementStatus(EMovementStatus::EMS_Normal);
			break;
		case EStaminaStatus::ESS_ExhaustedRecovering:
			if (Stamina + DeltaStamina >= MinSprintStamina)
			{
				SetStaminaStatus(EStaminaStatus::ESS_Normal);
				Stamina += DeltaStamina ;
			}
			else
			{
				Stamina += DeltaStamina ;
			}
			SetMovementStatus(EMovementStatus::EMS_Normal);
			break;
		default:
			;
	}

	if (bInterpToEnemy && CombatTarget)
	{

	}
}

FRotator GetLookAtRotationYaw(FVector Target)
{
	FRotator LookAtRotation
}

// Called to bind functionality to input
void AMain::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AMain::ShiftKeyDown);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AMain::ShiftKeyUP);

	PlayerInputComponent->BindAction("LMB", IE_Pressed, this, &AMain::LMBDown);
	PlayerInputComponent->BindAction("LMB", IE_Released, this, &AMain::LMBUp);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMain::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMain::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AMain::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AMain::LookUpAtRate);
}

void AMain::MoveForward(float value){

	if ((Controller != nullptr) && (value != 0.f) && (!bAttacking))
	{
		// find out which way is forward
		const FRotator Rotation = Controller -> GetControlRotation();
		const FRotator YawRotation(0.f,Rotation.Yaw,0.f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction,value);
	}
}

void AMain::MoveRight(float value){
	
	if ((Controller != nullptr) && (value != 0.f)&& (!bAttacking))
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

void AMain::LMBDown()
{
	bLMBDown = true;

	if (ActiveOverlappingItem)
	{
		AWeapon* Weapon = Cast<AWeapon>(ActiveOverlappingItem);
		if (Weapon)
		{
			Weapon -> Equip(this);
			SetActiveOverlappingItem(nullptr);
		}
	}
	else if (EquippedWeapon)
	{
		Attack();
	}
}

void AMain::LMBUp()
{
	bLMBDown = false;
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

void AMain::SetMovementStatus(EMovementStatus Status)
{
	MovementStatus = Status;
	if (MovementStatus == EMovementStatus::EMS_Sprinting)
	{
		GetCharacterMovement() -> MaxWalkSpeed = SprintingSpeed ;
	}
	else
	{
		GetCharacterMovement() -> MaxWalkSpeed = RunningSpeed ;
	}
}

void AMain::ShiftKeyDown()
{
	bShiftKeyDown = true ;
}

void AMain::ShiftKeyUP()
{
	bShiftKeyDown = false ;
}

void AMain::ShowPickupLocations()
{	

	// 둘다 똑같은 행위를 하는 반복문이지만, 쓰임새가 살짝 달라질 수 있음.

	// 1. 인덱스가 필요한 경우
	// for (int32 i = 0 ; i<PickupLocations.Num(); i++)
	// {
	// 	UKismetSystemLibrary::DrawDebugSphere(this, PickupLocations[i], 25.f , 12, FLinearColor::Green, 5.f, .5f );
	// }

	// 2. 인덱스가 필요하지 않고, 전체를 동시 실행할 경우
	for (FVector Location : PickupLocations )
	{
		UKismetSystemLibrary::DrawDebugSphere(this, Location, 25.f , 12, FLinearColor::Green, 5.f, .5f );
	}
		
}

void AMain::SetEquippedWeapon(AWeapon* WeaponToSet)
{	
	if (EquippedWeapon)
	{
		EquippedWeapon->Destroy();
	}
	EquippedWeapon = WeaponToSet;
}

void AMain::Attack()
{	
	if(!bAttacking)
	{
		bAttacking = true;
		SetInterpToEnemy(true);

		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance && CombatMontage)
		{
			int32 Section = FMath::RandRange(0,1);
			switch (Section)
			{
				case 0:
					AnimInstance->Montage_Play(CombatMontage, 2.2f);
					AnimInstance->Montage_JumpToSection(FName("Attack_1"), CombatMontage);
					break;
				case 1:
					AnimInstance->Montage_Play(CombatMontage, 1.5f);
					AnimInstance->Montage_JumpToSection(FName("Attack_2"), CombatMontage);
					break;
				default:
				    ;
			}
		}
	}
}

void AMain::AttackEnd()
{
	bAttacking = false;
	if(bLMBDown)
	{
		Attack();
	}
}

void AMain::PlaySwingSound()
{	
	if (EquippedWeapon -> SwingSound)
	{
		UGameplayStatics::PlaySound2D(this, EquippedWeapon->SwingSound);
	}

}

void AMain::SetInterpToEnemy(bool Interp)
{
	bInterpToEnemy = Interp;
}