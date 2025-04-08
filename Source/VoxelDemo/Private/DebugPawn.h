#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Chunk.h"
#include "DebugPawn.generated.h"

UCLASS()
class VOXELDEMO_API ADebugPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ADebugPawn();
	int CurrentDebugPage;
	void UpdateDebugDisplay();
	void NextDebugPage();
	static void ListAllActorTags(UWorld* World);

	UPROPERTY(EditAnywhere, Category="Debug")
	int32 DebugPageSize = 3;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	TArray<FVector> vertexData;
	FVector BaseLocation;
};
