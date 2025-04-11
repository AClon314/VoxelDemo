// Fill out your copyright notice in the Description page of Project Settings.


#include "VoxelFunctionLib.h"

FIntVector UVoxelFunctionLib::WorldToBlockPosition(const FVector& Position){
	return FIntVector(Position) / 100;
}

FIntVector UVoxelFunctionLib::WorldToLocalBlockPosition(const FVector& Position, const int Size){
	const auto ChunkPos = WorldToChunkPosition(Position, Size);

	auto Result = WorldToBlockPosition(Position) - ChunkPos * Size;

	// Negative Normalization
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
