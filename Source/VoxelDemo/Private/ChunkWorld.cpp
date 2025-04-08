// Fill out your copyright notice in the Description page of Project Settings.


#include "ChunkWorld.h"


// Sets default values
AChunkWorld::AChunkWorld(){
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AChunkWorld::BeginPlay(){
	Super::BeginPlay();
	
}

// Called every frame
void AChunkWorld::Tick(float DeltaTime){
	Super::Tick(DeltaTime);
}

