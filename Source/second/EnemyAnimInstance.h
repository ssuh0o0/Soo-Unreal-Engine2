// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class SECOND_API UEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	
	virtual void NativeInitializeAnimation() override;

	UFUNCTION(BlueprintCallable, Category = AnimationProperty)
	void UpdateAniamtionProperties();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	float MovementSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	class APawn* Pawn;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	class AEnemy* Enemy;

};
