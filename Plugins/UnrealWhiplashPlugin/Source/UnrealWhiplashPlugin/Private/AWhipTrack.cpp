// Fill out your copyright notice in the Description page of Project Settings.


#include "AWhipTrack.h"

// Sets default values
AAWhipTrack::AAWhipTrack()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAWhipTrack::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAWhipTrack::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

