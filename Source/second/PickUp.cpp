// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUp.h"
#include "Main.h"

APickUp::APickUp()
{
    CoinCount = 1 ;
}

void APickUp::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
    Super:: OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult );
    UE_LOG(LogTemp, Warning, TEXT("APickUp:: overlap begin"));

    if (OtherActor)
    {
        AMain* Main = Cast<AMain>(OtherActor);
        if (Main)
        {
            Main -> IncrementCoin(CoinCount);
            Main -> PickupLocations.Add(GetActorLocation());

            Destroy();
        }
    }
}	

void APickUp::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) 
{
    Super:: OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
    UE_LOG(LogTemp, Warning, TEXT("APickUp:: overlap end"));
}