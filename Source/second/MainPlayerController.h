// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SECOND_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	/** 에디터에서 UMG 에셋을 참조하기 위함 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<class UUserWidget> HUDOverlayAsset;

	/** 생성 후 위젯을 선택하기 위한 변수 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	UUserWidget* HUDOverlay;

protected:

	virtual void BeginPlay() override ;

	
};
