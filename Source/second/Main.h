// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Main.generated.h"

UCLASS()
class SECOND_API AMain : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMain();

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

	FORCEINLINE class USpringArmComponent* GetCameraBoom() const {return CameraBoom ;}
	FORCEINLINE class UCameraComponent* GetFoloowCamera() const {return FollowCamera;}

};
