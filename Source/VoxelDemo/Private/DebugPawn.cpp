#include "DebugPawn.h"
#include "Chunk.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ADebugPawn::ADebugPawn(){
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// 设置默认值
	CurrentDebugPage = 0;
}

void ADebugPawn::ListAllActorTags(UWorld* World){
	TArray<AActor*> AllActors;
	UGameplayStatics::GetAllActorsOfClass(World, AActor::StaticClass(), AllActors);

	for (AActor* Actor : AllActors){
		FString ActorName = Actor->GetName();
		FString Tags;
		for (const FName& Tag : Actor->Tags){
			Tags += Tag.ToString() + TEXT(" ");
		}
		UE_LOG(LogTemp, Log, TEXT("Actor: %s, Tags: %s"), *ActorName, *Tags);
	}
}

// Called when the game starts or when spawned
void ADebugPawn::BeginPlay(){
	Super::BeginPlay();
	ListAllActorTags(GetWorld());

	TArray<AActor*> ChunkActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Chunk"), ChunkActors);
	if (ChunkActors.Num() > 0){
		UE_LOG(LogTemp, Display, TEXT("Found %d Chunks with the tag 'Chunk'"), ChunkActors.Num());
		if (AChunk* Chunk = Cast<AChunk>(ChunkActors[0])){
			vertexData = Chunk->VertexData;
			BaseLocation = Chunk->GetActorLocation();
			UE_LOG(LogTemp, Display, TEXT("Found one Chunk with the tag 'Chunk'"));
		}
	}
	else{
		UE_LOG(LogTemp, Error, TEXT("No Chunk found with the tag 'Chunk'"));
	}
	UpdateDebugDisplay();
}

// Called every frame
void ADebugPawn::Tick(float DeltaTime){
	Super::Tick(DeltaTime);
}

void ADebugPawn::UpdateDebugDisplay(){
	// 清除所有持久调试信息
	// FlushPersistentDebugLines(GetWorld());
	FlushDebugStrings(GetWorld());

	for (int32 i = 0; i < DebugPageSize; ++i){
		int32 Index = CurrentDebugPage * DebugPageSize + i;
		if (Index >= vertexData.Num()){
			UE_LOG(LogTemp, Error, TEXT("Index is out of bounds"));
			break;
		}

		// 垂直向下排列，每个间隔50单位
		FVector TextPosition = BaseLocation + vertexData[Index] + FVector(0, 0, 0); //-50.0f * i
		FString Text = FString::Printf(TEXT("%d"), Index);
		FColor Color = FColor::MakeRandomColor();

		DrawDebugString(
			GetWorld(),
			TextPosition,
			Text,
			nullptr,
			Color,
			-1.0f, // 持续1000秒
			false,
			2.0f
		);
	}
	UE_LOG(LogTemp, Log, TEXT("UpdateDebugDisplay: %d"), vertexData.Num());
}

void ADebugPawn::NextDebugPage(){
	int32 length = vertexData.Num();
	int32 MaxPage = FMath::Max(0, (length - 1) / DebugPageSize);
	CurrentDebugPage++;

	// 循环到第一页
	if (CurrentDebugPage > MaxPage){
		CurrentDebugPage = 0;
	}

	UpdateDebugDisplay();
	UE_LOG(LogTemp, Log, TEXT("CurrentDebugPage: %d"), CurrentDebugPage);
}

// Called to bind functionality to input
void ADebugPawn::SetupPlayerInputComponent(UInputComponent* InputComponent){
	Super::SetupPlayerInputComponent(InputComponent);
	InputComponent->BindAction("NextDebugPage", IE_Pressed, this, &ADebugPawn::NextDebugPage);
}
