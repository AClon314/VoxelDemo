// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Chunk.generated.h"

enum class EBlock;
enum class EDirection;
class FastNoiseLite;
class UProceduralMeshComponent;
// 前置声明: 主要是为了避免引入不必要的头文件，从而减少编译时间。例如，如果一个类只需要知道另一个类的存在而不需要知道其内部实现细节，就可以只做前置声明。
// 头文件: 包含了详细的类型定义、函数原型等信息，使得其他源文件可以通过包含这些头文件来使用其中定义的功能。

UCLASS()
class AChunk : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AChunk();
	UPROPERTY(EditAnywhere, Category="Chunk")
	int Size = 32;

	UPROPERTY(EditAnywhere, Category="Chunk")
	int Scale = 1;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	/* 在 C++ 中，virtual 关键字用于声明虚函数（virtual function）。虚函数主要用于实现多态性（polymorphism），它是面向对象编程中的一个重要概念。下面是一些关键点：
多态性：虚函数允许派生类重写基类中的函数，从而使得基类指针或引用可以调用派生类中的函数。这样可以在运行时根据对象的实际类型来决定调用哪个函数版本。
动态绑定：虚函数的调用是在运行时通过虚函数表（vtable）进行的，而不是在编译时确定的。这意味着即使在编译时只知道基类的指针或引用，也可以在运行时调用派生类的具体实现。
覆盖（override）：当派生类中有一个函数与基类中的虚函数具有相同的签名时，这个函数就覆盖了基类的虚函数。使用 override 关键字可以确保覆盖的是正确的虚函数，并且编译器会在编译时进行检查。
纯虚函数：可以通过将虚函数声明为 = 0 来创建纯虚函数。纯虚函数没有实际的实现，必须在派生类中提供具体的实现。含有纯虚函数的类通常是抽象类，不能直接实例化。 */

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	TObjectPtr<UProceduralMeshComponent> Mesh;
	TObjectPtr<FastNoiseLite> Noise;
	// UE4直接使用指针；UE5 使用TObjectPtr

	TArray<EBlock> Blocks;

	TArray<FVector> VertexData;
	TArray<int32> TriangleData;
	TArray<FVector2d> UVData;
	// 点、面、UV

	int VertexCount = 0;

	const int I0 = 0;
	const int I100 = 100;
	const FVector BlockVertexData[8] = {
		FVector(I100, I100, I100),
		FVector(I100,I0,I100),
		FVector(I100,I0,I0),
		FVector(I100,I100,I0),
		FVector(I0,I0,I100),
		FVector(I0,I100,I100),
		FVector(I0,I100,I0),
		FVector(I0,I0,I0)
	};

	const int BlockTriangleData[24] = {
		0, 1, 2, 3, // Front
		5, 0, 3, 6, // Right
		4, 5, 6, 7, // Back
		1, 4, 7, 2, // Left
		5, 4, 1, 0, // Up
		3, 2, 7, 6 // Down
	};

	// 根据噪声高度图，生成方块
	void GenerateBlocks();

	// 生成顶点数据
	void GenerateMesh();
	
	//将顶点数据与index数据应用到 ProceduralMeshComponent
	void ApplyMesh() const;
	/* c++98 特性：当一个成员函数被声明为const时，这个函数承诺不会改变调用它的对象的状态。这对于常量对象特别有用，因为它们只能调用const的成员函数。
	 *
	 * class MyClass {
	 * public:
	 * void PrintValue() const;  // 声明为const成员函数
	 * void SetValue(int value); // 不是const成员函数
	 * 
	 * const MyClass constObj;
	 * constObj.PrintValue();  // 常量对象可以调用const成员函数
	 * // constObj.SetValue(42);  // 编译错误：不能调用非const成员
	 */

	// 是否包含透明方块
	bool Check(FVector Position) const;
	
	// 将顶点数据添加到index data
	void CreateFace(EDirection Direction, FVector Position);
	
	// utility method使用方法：顶点查表
	TArray<FVector> GetFaceVertices(EDirection Direction, FVector Position) const;

	// 指定方向上的相邻位置，用于给位置添加方向向量
	FVector GetPositionInDirection(EDirection Direction, FVector Position) const;
	
	// 将3维坐标转换为一维索引
	int GetBlockIndex(int X, int Y, int Z) const;
	
};
