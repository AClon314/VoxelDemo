// Fill out your copyright notice in the Description page of Project Settings.


#include "VoxelFunctionLib.h"

FIntVector UVoxelFunctionLib::WorldToBlockPosition(const FVector& Position){
	return FIntVector(Position) / 100;
}

FIntVector UVoxelFunctionLib::WorldToLocalBlockPosition(const FVector& Position, const int Size){
	const auto ChunkPos = WorldToChunkPosition(Position, Size);

	auto Result = WorldToBlockPosition(Position) - ChunkPos * Size;

	// Negative Normalization: 2/32 == 0, but -2/32 == 0 pointed to the same chunk, so we need offset -1 to the negative side
	if (ChunkPos.X < 0){
		Result.X--;
	}
	if (ChunkPos.Y < 0){
		Result.Y--;
	}
	if (ChunkPos.Z < 0){
		Result.Z--;
	}

	return Result;
}

FIntVector UVoxelFunctionLib::WorldToChunkPosition(const FVector& Position, const int Size){
	FIntVector Result;

	const int Factor = Size * 100;
	const auto IntPosition = FIntVector(Position);

	if (IntPosition.X < 0){
		Result.X = static_cast<int>(Position.X / Factor) - 1;
	}
	else{
		Result.X = static_cast<int>(Position.X / Factor);
	}


	if (IntPosition.Y < 0){
		Result.Y = static_cast<int>(Position.Y / Factor) - 1;
	}
	else{
		Result.Y = static_cast<int>(Position.Y / Factor);
	}

	if (IntPosition.Z < 0){
		Result.Z = static_cast<int>(Position.Z / Factor) - 1;
	}
	else{
		Result.Z = static_cast<int>(Position.Z / Factor);
	}

	return Result;
}

FIntVector UVoxelFunctionLib::GetMatrixData(const FIntVector& Position, const int Size){
	FIntVector Result;

	Result.X = Position.X % Size;
	Result.Y = Position.Y % Size;
	Result.Z = Position.Z % Size;

	if (Result.X < 0){
		Result.X += Size;
	}
	if (Result.Y < 0){
		Result.Y += Size;
	}
	if (Result.Z < 0){
		Result.Z += Size;
	}

	return Result;
}
