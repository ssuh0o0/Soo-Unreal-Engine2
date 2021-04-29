// Fill out your copyright notice in the Description page of Project Settings.


#include "ColliderPawnMovement.h"

void UColliderPawnMovement::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType,ThisTickFunction);

    //종료해야하는지 확인하는 조건
    if(!PawnOwner || !UpdatedComponent || ShouldSkipUpdate(DeltaTime)){
        return;
    }

    //ConsumeInputVector 입력받은 벡터를 반환한 후 0으로 재설정
    FVector DesiredMovementThisFrame = ConsumeInputVector().GetClampedToMaxSize(1.0f);

    if(!DesiredMovementThisFrame.IsNearlyZero()){ 

        FHitResult Hit;
        //sweep 이란? 우리가 움직이고 뭔가를 쳤는지 확인하려면 필요한 것
        SafeMoveUpdatedComponent(DesiredMovementThisFrame, UpdatedComponent->GetComponentRotation(),true ,Hit);

        //만약 우리가 무언가에 부딪히면, 그  side로 미끄러지게 하자.
        if (Hit.IsValidBlockingHit()){

            //parameter 설명 : DesiredMovementThisFrame -> 움직이고 싶은 움직임의 양, 1.f - Hit.Time -> 걸리는 시간, Hit.Normal->충돌 방향
            SlideAlongSurface(DesiredMovementThisFrame, 1.f - Hit.Time, Hit.Normal, Hit );
            UE_LOG( LogTemp, Warning, TEXT("ValidBlocking hit"));
        }
    }
}
