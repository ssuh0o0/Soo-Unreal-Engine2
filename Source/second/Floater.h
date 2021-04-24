// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Floater.generated.h"

UCLASS()
class SECOND_API AFloater : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFloater();

	UPROPERTY(BlueprintReadWrite,Category = "ActorMeshComponents")
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Floater Variables")
	FVector InitialLocation = FVector(0.0f, 0.0f, 0.0f);

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Floater Variables")
	FVector PlacedLocation;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Floater Variables")
	FVector WorldOrigin;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Floater Variables")
	FVector InitialDirection;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Floater Variables")
	bool bShouldFloat;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Floater Variables")
	bool bInitializeFloaterLocation;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
