// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Components/SphereComponent.h"
#include "AIController.h"
#include "Main.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Sound/SoundCue.h"
#include "Animation/AnimInstance.h"
#include "TimerManager.h"
#include "Components/CapsuleComponent.h"

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

	CombatCollision = CreateDefaultSubobject<UBoxComponent>((TEXT("CombatCollision")));
	CombatCollision -> AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("EnemySocket"));

	bOverlappingCombatSphere = false;

	Health = 75.f;
	MaxHealth = 100.f ;
	Damage = 10.f ;

	AttackMinTime = 0.5f;
	AttackMaxTime = 3.5f;

	EnemyMovementStatus = EEnemyMovementStatus::EMS_Idle;

	DeathDelay = 3.f;
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	AIController = Cast<AAIController>(GetController());

	AgroSphere -> OnComponentBeginOverlap.AddDynamic(this, &AEnemy::AgroSphereOnOverlapBegin);
	AgroSphere -> OnComponentEndOverlap.AddDynamic(this, &AEnemy::AgroSphereOnOverlapEnd);
	
	CombatSphere -> OnComponentBeginOverlap.AddDynamic(this, &AEnemy::CombatSphereOnOverlapBegin);
	CombatSphere -> OnComponentEndOverlap.AddDynamic(this, &AEnemy::CombatSphereOnOverlapEnd);
	
	CombatCollision->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::CombatOnOverlapBegin);
	CombatCollision->OnComponentEndOverlap.AddDynamic(this, &AEnemy::CombatOnOverlapEnd);

	CombatCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    CombatCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
    CombatCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    CombatCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn,ECollisionResponse::ECR_Overlap);
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

void AEnemy::AgroSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor && Alive())
	{
		AMain* Main = Cast<AMain>(OtherActor);
		if(Main)
		{
			MoveToTarget(Main);
		}
	}
}

void AEnemy::AgroSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		AMain* Main = Cast<AMain>(OtherActor);
		if(Main)
		{	
			SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Idle);
			if (AIController)
			{
				AIController -> StopMovement();
			}
		}
	}
}

void AEnemy::CombatSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor && Alive())
	{
		AMain* Main = Cast<AMain>(OtherActor);
		if(Main)
		{	
			Main -> SetCombatTarget(this);	
			CombatTarget = Main ;
			bOverlappingCombatSphere = true;
			Attack();
		}
	}
}	

void AEnemy::CombatSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		AMain* Main = Cast<AMain>(OtherActor);
		if(Main)
		{	
			if (Main->CombatTarget == this)
			{
				Main -> SetCombatTarget(nullptr);	
			}
			bOverlappingCombatSphere =false;
			
			/** BlueprintCallable 해서 bOverlappingCombatSphere 이용하면 굳이 이렇게 안써도 됨) */
			// SetEnemyMovementStatus(EEnemyMovementStatus::EMS_MoveToTarget);
			// MoveToTarget(Main);

			if (EnemyMovementStatus != EEnemyMovementStatus::EMS_Attacking ) 
			{
				MoveToTarget(Main);
				CombatTarget = nullptr ;
			}
			GetWorldTimerManager().ClearTimer(AttackTimer);
		}
	}
}

void AEnemy::MoveToTarget(class AMain* Target)
{
	SetEnemyMovementStatus(EEnemyMovementStatus::EMS_MoveToTarget);

	if(AIController)
	{
		FAIMoveRequest MoveRequest;
		MoveRequest.SetGoalActor(Target);
		MoveRequest.SetAcceptanceRadius(10.0f);

		FNavPathSharedPtr NavPath;

		AIController-> MoveTo(MoveRequest, &NavPath);

		// auto PathPoints = NavPath -> GetPathPoints();
		// for (auto Point : PathPoints)
		// {
		// 	FVector Location = Point.Location;

		// 	UKismetSystemLibrary::DrawDebugSphere(this, Location, 25.f, 8, FLinearColor::Red, 10.f, 1.5f);
		// }
	}
}

void AEnemy::CombatOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
    if (OtherActor)
    {
        AMain* Main = Cast<AMain>(OtherActor);
        if(Main)
        {
            if(Main -> HitParticles)
            {
                const USkeletalMeshSocket* TipSocket = GetMesh() -> GetSocketByName("TipSocket");
                if(TipSocket)
                {
                    FVector SocketLocation = TipSocket->GetSocketLocation(GetMesh());
                    UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),Main -> HitParticles, SocketLocation, FRotator(0.f), false );
                }
                
            }
            if (Main->HitSound)
		    {
			    UGameplayStatics::PlaySound2D(this, Main->HitSound);
		    }
			if (DamageTypeClass)
			{
				UGameplayStatics::ApplyDamage(Main, Damage, AIController, this, DamageTypeClass);
			}
        }
    }
} 

void AEnemy::CombatOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

void AEnemy::ActivateCollision()
{
    CombatCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AEnemy::DeactivateCollision()
{
    CombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AEnemy::Attack()
{
	if (Alive()){
		if (AIController)
		{
			AIController -> StopMovement();
			SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Attacking);
		}

		if (!bAttacking)
		{
			bAttacking = true ;
			UAnimInstance* AnimInstatnce = GetMesh() -> GetAnimInstance();
			if (AnimInstatnce)
			{
				AnimInstatnce->Montage_Play(CombatMontage, 1.35f);
				AnimInstatnce->Montage_JumpToSection(FName("Attack"), CombatMontage);
			}
			if (SwingSound)
			{
				UGameplayStatics::PlaySound2D(this, SwingSound);
			}
		}
	}
}

void AEnemy::AttackEnd()
{
	bAttacking = false;
	if (bOverlappingCombatSphere)
	{	
		float AttackTime = FMath::FRandRange(AttackMinTime,AttackMaxTime);
		GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemy::Attack, AttackTime);
		Attack();
	}
}

float AEnemy::TakeDamage(float DamageAmount,struct FDamageEvent const & DamageEvent,class AController * EventInstigator, AActor * DamageCauser)
{
	if(Health - DamageAmount <= 0.f)
	{
		Health -= DamageAmount;
		Die();
	}else{
		Health -= DamageAmount;
	}

	return DamageAmount;
}

void AEnemy::Die()
{
	UAnimInstance* AnimInstatnce = GetMesh() -> GetAnimInstance();
	if (AnimInstatnce)
	{
		AnimInstatnce->Montage_Play(CombatMontage, 1.35f);
		AnimInstatnce->Montage_JumpToSection(FName("Death"), CombatMontage);
	}
	SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Dead);

	CombatCollision -> SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AgroSphere -> SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CombatSphere -> SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent() -> SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AEnemy::DeathEnd()
{
	GetMesh() -> bPauseAnims = true;
	GetMesh() -> bNoSkeletonUpdate = true;

	GetWorldTimerManager().SetTimer(DeathTimer, this, &AEnemy::Disappear, DeathDelay);
}

bool AEnemy::Alive()
{
	return GetEnemyMovementStatus() != EEnemyMovementStatus::EMS_Dead;
}

void AEnemy::Disappear()
{
	Destroy();
}