// Fill out your copyright notice in the Description page of Project Settings.
#include "Chunk.h"

#include "EnhancedInputComponent.h"
#include "Enums.h"
#include "ProceduralMeshComponent.h"
#include "FastNoiseLite.h"


AChunk::AChunk(){
	Tags.Add(FName("Chunk"));
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UProceduralMeshComponent>("Mesh");
	Noise = new FastNoiseLite();
	Noise->SetFrequency(0.03f);
	Noise->SetNoiseType(FastNoiseLite::NoiseType_Perlin);
	Noise->SetFractalType(FastNoiseLite::FractalType_FBm);

	Blocks.SetNum(Size * Size * Size);
	// init blocks. Resize the blocks array to the cube of the chunk size.
	// better to resize it AT ONCE, rather than expanding during the runtime.

	Mesh->SetCastShadow(false);
	// mesh settings
	// for baked AO(Ambient occlusion)
}

// Called when the game starts or when spawned
void AChunk::BeginPlay(){
	Super::BeginPlay();

	GenerateBlocks();
	GenerateMesh();
	ApplyMesh();

	UpdateDebugDisplay();
}

void AChunk::GenerateBlocks(){
	const auto Location = GetActorLocation();
	Blocks[GetBlockIndex(0, 0, 0)] = EBlock::Stone;

	for (int x = 0; x < Size; x++){
		for (int y = 0; y < Size; y++){
			// Unreal的默认最小单位是100cm
			const float Xpos = (x * I100 + Location.X) / I100;
			const float Ypos = (y * I100 + Location.Y) / I100;
			// 除法后移，减少精度损失

			float iNoise = Noise->GetNoise(Xpos, Ypos);
			const int Height = FMath::Clamp(FMath::RoundToInt((Noise->GetNoise(Xpos, Ypos) + 1) * Size / 2),
			                                0, Size);
			UE_LOG(LogTemp, Log, TEXT("Height(%f,%f):\t%d"), Xpos, Ypos, Height);
			// clamp是为了防下标越界

			for (int z = 0; z < Height; z++){
				Blocks[GetBlockIndex(x, y, z)] = EBlock::Stone;
				// hard-coded AWARE!
			}

			for (int z = Height; z < Size; z++){
				Blocks[GetBlockIndex(x, y, z)] = EBlock::Air;
			}
		}
	}
}

void AChunk::GenerateMesh(){
	for (int x = 0; x < Size; x++){
		for (int y = 0; y < Size; y++){
			for (int z = 0; z < Size; z++){
				if (Blocks[GetBlockIndex(x, y, z)] > EBlock::Air){
					const auto Position = FVector(x, y, z);
					for (EDirection Direction : TEnumRange<EDirection>()){
						// 如果这个方向的相邻方块为透明，则创建一个面
						if (Check(GetPositionInDirection(Direction, Position))){
							CreateFace(Direction, Position * I100);
						}
					}
				}
			}
		}
	}
}

void AChunk::ApplyMesh() const{
	Mesh->CreateMeshSection(0, VertexData, TriangleData, TArray<FVector>(), UVData, TArray<FColor>(),
	                        TArray<FProcMeshTangent>(), false);
}

bool AChunk::Check(const FVector Position) const{
	if (Position.X >= Size || Position.Y >= Size || Position.Z >= Size ||
		Position.X < 0 || Position.Y < 0 || Position.Z < 0){
		return true; // 位置越界区块，则当透明方块
	}
	return Blocks[GetBlockIndex(Position.X, Position.Y, Position.Z)] <= EBlock::Air;
}

void AChunk::CreateFace(const EDirection Direction, const FVector Position){
	TArray<FVector> FaceVertices = GetFaceVertices(Direction, Position);
	VertexData.Append(FaceVertices);
	UVData.Append({FVector2D(0, 0), FVector2D(1, 0), FVector2D(1, 1), FVector2D(0, 1)}); // 这里的顺序有关系吗 ?!
	TriangleData.Append({
		// 计算机图形学，三角形顶点的排列顺序决定了面的法线方向。逆时针面朝外（法线正向），顺时针面朝内.
		VertexCount + 3, VertexCount + 2, VertexCount, VertexCount + 2, VertexCount + 1, VertexCount
	}); // 一个四边面，6个顶点

	VertexCount += 4; // 下一次遍历
}

TArray<FVector> AChunk::GetFaceVertices(EDirection Direction, FVector Position) const{
	TArray<FVector> Vertices;

	for (int i = 0; i < 4; i++){
		FVector VertexPos = Position + BlockVertexData[BlockTriangleData[i + static_cast<int>(Direction) * 4]] * Scale;
		Vertices.Add(VertexPos);

		// FString VertexIndexString = FString::Printf(TEXT("%d"), VertexCount + i);
		// DrawDebugString(GetWorld(), VertexPos, VertexIndexString, nullptr, FColor::Red, -1, false, 2);
	}
	return Vertices;
}

/**
 * 根据指定方向计算相对于当前位置的新位置。
 * 
 * 该函数接收一个方向枚举值和一个三维向量位置作为参数，根据方向的不同，
 * 在输入的位置基础上加上一个代表该方向的单位向量，从而计算出新的位置。
 * 如果方向参数无效（即不在枚举定义的方向范围内），则抛出异常。
 * 
 * @param Direction 指定的方向，类型为EDirection枚举。
 * @param Position 起始位置，类型为FVector。
 * @return 根据方向计算得到的新位置。
 * @exception std::invalid_argument 当方向参数无效时抛出此异常。
 */
FVector AChunk::GetPositionInDirection(const EDirection Direction, const FVector Position) const{
	switch (Direction){
	case EDirection::Front: return Position + FVector(1, 0, 0);
	case EDirection::Back: return Position + FVector(-1, 0, 0);
	case EDirection::Left: return Position + FVector(0, -1, 0);
	case EDirection::Right: return Position + FVector(0, 1, 0);
	case EDirection::Up: return Position + FVector(0, 0, 1);
	case EDirection::Down: return Position + FVector(0, 0, -1);
	default: throw std::invalid_argument("Invalid direction");
	}
}

/**
 * flatten 3d pos to 1d index
 */
int AChunk::GetBlockIndex(int X, int Y, int Z) const{
	return Z * Size * Size + Y * Size + X;
}


// Called every frame
void AChunk::Tick(float DeltaTime){
	Super::Tick(DeltaTime);
}

void AChunk::UpdateDebugDisplay(){
	// 清除所有持久调试信息
	// FlushPersistentDebugLines(GetWorld());
	FlushDebugStrings(GetWorld());
	constexpr int32 Multi = 120;

	for (int32 i = 0; i < DebugPageSize; ++i){
		int32 Index = CurrentDebugPage * DebugPageSize + i;
		if (Index >= VertexData.Num()){
			UE_LOG(LogTemp, Error, TEXT("Index is out of bounds"));
			break;
		}

		// 垂直向下排列，每个间隔50单位
		float offset = ((i % 8) - 4) * 2.0f;
		FVector TextPosition = GetActorLocation() + VertexData[Index] + FVector(offset, offset, offset); //-50.0f * i
		FString Text = FString::Printf(TEXT("%d"), Index);
		int32 Face = Index / 3;
		FColor Color = FColor(
			FMath::Clamp((Face) * Multi + Multi, Multi, 255),
			FMath::Clamp((Face % 2) * Multi + Multi, Multi, 255),
			FMath::Clamp((Face / 2 % 2) * Multi + Multi, Multi, 255)
		);

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
	UE_LOG(LogTemp, Log, TEXT("UpdateDebugDisplay: %d"), VertexData.Num());
}

void AChunk::NextDebugPage(){
	int32 length = VertexData.Num();
	int32 MaxPage = FMath::Max(0, (length - 1) / DebugPageSize);
	CurrentDebugPage++;

	// 循环到第一页
	if (CurrentDebugPage > MaxPage){
		CurrentDebugPage = 0;
	}

	UpdateDebugDisplay();
	UE_LOG(LogTemp, Log, TEXT("CurrentDebugPage: %d"), CurrentDebugPage);
}

void AChunk::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent){
	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	Input->BindAction(NextDebugPageAction, ETriggerEvent::Triggered, this, &AChunk::NextDebugPage);
	UE_LOG(LogTemp, Log, TEXT("SetupPlayerInputComponent"));
}
