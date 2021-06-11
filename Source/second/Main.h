// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Main.generated.h"

UENUM(BlueprintType)
enum class EMovementStatus : uint8
{
	EMS_Normal UMETA(DisplayName = "Normal"),
	EMS_Sprinting UMETA(DisplayName = "Sprinting"),
	EMS_MAX UMETA(DisplayName = "DefaultMax")
};

UENUM(BlueprintType)
enum class EStaminaStatus : uint8
{
	ESS_Normal UMETA(DisplayName = "Normal"),
	ESS_BelowMin UMETA(DisplayName = "BelowMin"),
	ESS_Exhausted UMETA(DisplayName = "Exhausted"),
	ESS_ExhaustedRecovering UMETA(DisplayName = "ExhaustedRecovering"),
	ESS_MAX UMETA(DisplayName = "DefaultMax")
};

UCLASS()
class SECOND_API AMain : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMain();

	TArray<FVector> PickupLocations;

	UFUNCTION(BlueprintCallable)
	void ShowPickupLocations();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite ,Category = "Enums")
	EMovementStatus MovementStatus;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite ,Category = "Enums")
	EStaminaStatus StaminaStatus;

	FORCEINLINE void SetStaminaStatus(EStaminaStatus Status) { StaminaStatus = Status;}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = " Movement ")
	float StaminaDrainRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = " Movement ")
	float MinSprintStamina;

	/** 동작 상태와 달리는 속도를 설정하는 함수  */
	void SetMovementStatus(EMovementStatus Status);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Running")
	float RunningSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Running")
	float SprintingSpeed;

	bool bShiftKeyDown ;

	/** 스프린팅을 가능하게 down 키를 누름 */
	void ShiftKeyDown();
	/** 스프린팅을 멈추도록 함 */
	void ShiftKeyUP();

	/** Camera Boom positioning the camera behind the player */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = camera, meta=(AllowPrivateAccess="true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = camera, meta=(AllowPrivateAccess="true"))
	class UCameraComponent* FollowCamera;

	/** Base turn rates to scale turning fumctions for the camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = camera)
	float BaseTurnRate;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = camera)
	float BaseLookUpRate;

	/**
	 * 
	 *  Player stats
	 * 
	 */

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Player Stats")
	float MaxHealth ;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Player Stats")
	float Health ;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Player Stats")
	float MaxStamina;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Player Stats")
	float Stamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "Player Stats")
	int32 Coins; 

	void DecrementHealth(float Amount);

	void IncrementCoin(int32 Amount);

	void Die();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Called for forwards/backwards input */
	void MoveForward(float value);

	/** Called for left/right input */
	void MoveRight(float value);

	/** Called via input to turn at a given rate
	 * @param Rate  This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/** Called via input to look up/down at a given rate
	 * @param Rate  This is a normalized rate, i.e. 1.0 means 100% of desired look up/down rate
	 */
	void LookUpAtRate(float Rate);

	// 왼쪽 마우스 버튼이 눌렸는지에 대한 변수들
	bool bLMBDown;
	void LMBDown();
	void LMBUp();



	FORCEINLINE class USpringArmComponent* GetCameraBoom() const {return CameraBoom ;}
	FORCEINLINE class UCameraComponent* GetFoloowCamera() const {return FollowCamera;}

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "items")
	class AWeapon* EquippedWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "items")
	class AItem* ActiveOverlappingItem;

	void SetEquippedWeapon(AWeapon* WeaponToSet);
	FORCEINLINE AWeapon* GetEquippedWeapon(){ return EquippedWeapon ; }
	FORCEINLINE void SetActiveOverlappingItem(AItem* Item) { ActiveOverlappingItem = Item ; }

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= " Anims")
	bool bAttacking;

	void Attack();

	UFUNCTION(BlueprintCallable)
	void AttackEnd();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Anims")
	class UAnimMontage* CombatMontage;
};
