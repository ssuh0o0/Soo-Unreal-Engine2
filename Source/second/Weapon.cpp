// Fill out your copyright notice in the Description page of Project Settings.



#include "Weapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Main.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

AWeapon::AWeapon()
{
    SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
    SkeletalMesh -> SetupAttachment(GetRootComponent());

    bWeaponParticle = false;

    WeaponState = EWeaponState::EWS_Pickup;
}

void AWeapon::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
    Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
    if ((WeaponState == EWeaponState::EWS_Pickup) && OtherActor)
    {
        AMain* Main = Cast<AMain>(OtherActor);
        if(Main)
        {   
            //장비 장착
            // Equip(Main);

            Main -> SetActiveOverlappingItem(this);
        }
    }
}

void AWeapon::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
    if (OtherActor)
    {
        AMain* Main = Cast<AMain>(OtherActor);
        if(Main)
        {   
            // 장비와 겹치자마자 , 잇다면 nullptr로 바꿔줌
            Main -> SetActiveOverlappingItem(nullptr);
        }
    }
}

void AWeapon::Equip(AMain* Char)
{
    if (Char)
    {
        SkeletalMesh -> SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
        SkeletalMesh -> SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

        SkeletalMesh -> SetSimulatePhysics(false);
        const USkeletalMeshSocket* LeftHandSocket = Char-> GetMesh()->GetSocketByName("LHSocket");

        if (LeftHandSocket)
        {
            LeftHandSocket -> AttachActor(this, Char ->GetMesh());
            bRotate = false;

            Char -> SetEquippedWeapon(this);
            Char -> SetActiveOverlappingItem(nullptr);
        }
        if (OnEquipSound) UGameplayStatics::PlaySound2D(this, OnEquipSound);
        if (!bWeaponParticle)
        {
            IdleParticlesComponent->Deactivate();
        }
    }
}