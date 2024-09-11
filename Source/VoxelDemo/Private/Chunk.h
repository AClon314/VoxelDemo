// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Chunk.generated.h"

enum class EBlock;
enum class EDirection;
class FastNoiseLite;
class UProceduralMeshComponent;

UCLASS()
class AChunk : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AChunk();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	/* 在 C++ 中，virtual 关键字用于声明虚函数（virtual function）。虚函数主要用于实现多态性（polymorphism），它是面向对象编程中的一个重要概念。下面是一些关键点：
多态性：虚函数允许派生类重写基类中的函数，从而使得基类指针或引用可以调用派生类中的函数。这样可以在运行时根据对象的实际类型来决定调用哪个函数版本。
动态绑定：虚函数的调用是在运行时通过虚函数表（vtable）进行的，而不是在编译时确定的。这意味着即使在编译时只知道基类的指针或引用，也可以在运行时调用派生类的具体实现。
覆盖（override）：当派生类中有一个函数与基类中的虚函数具有相同的签名时，这个函数就覆盖了基类的虚函数。使用 override 关键字可以确保覆盖的是正确的虚函数，并且编译器会在编译时进行检查。
纯虚函数：可以通过将虚函数声明为 = 0 来创建纯虚函数。纯虚函数没有实际的实现，必须在派生类中提供具体的实现。含有纯虚函数的类通常是抽象类，不能直接实例化。 */

	UPROPERTY(EditAnywhere, Category="Chunk")
	int32 Size = 32;

	UPROPERTY(EditAnywhere, Category="Chunk")
	float Scale = 1.0f;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
