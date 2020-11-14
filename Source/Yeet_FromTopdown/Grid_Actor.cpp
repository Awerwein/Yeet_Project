// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid_Actor.h"
#include "Components/SceneComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Components/StaticMeshComponent.h"
//#include "ProceduralMeshComponent.h"
#include "Math/Vector.h"

//class USceneComponent* RootSceneComponent;


// Sets default values
AGrid_Actor::AGrid_Actor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(("RootSceneComponent"));
	RootComponent = RootSceneComponent;

	GridMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Grid Mesh"));
	SelectionMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Selection Mesh"));


}

// Called when the game starts or when spawned
void AGrid_Actor::BeginPlay()
{
	Super::BeginPlay();
	




}

// Called every frame
void AGrid_Actor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGrid_Actor::CreateLine(const FVector Start, const FVector End, const float& Thickness, TArray<FVector>& Vertices, TArray<int32>& Triangles)
{
	float HalfThickness = Thickness / 2.0f;
	FVector LineVector;
	LineVector = End - Start;
	(&LineVector)->Normalize(0.0001f);
	//(&LineVector)->CrossProduct();

	FVector ThicknessDirection = LineVector ^ FVector(0.0f, 0.0f, 1.0f);
	//ThicknessDirection->CrossProduct(LineVector, FVector(0.0f, 0.0f, 1.0f));


	int32 Size = Vertices.Num();
	//TArray<int32> NewVerticesIndex = {Size+2, Size +1, Size, Size +2, Size +3, Size +1};
	Triangles += {Size + 2, Size + 1, Size, Size + 2, Size + 3, Size + 1};

	FVector Zero = Start + (ThicknessDirection * HalfThickness);
	FVector Two = Start - (ThicknessDirection * HalfThickness);
	FVector One = End + (ThicknessDirection * HalfThickness);
	FVector Three = End - (ThicknessDirection * HalfThickness);

	Vertices += {Zero, One, Two, Three};


}

void AGrid_Actor::DrawGrid()
{


	float LineEnd = 0.0f;
	float X_i = 0.0f;
	float Y_i = 0.0f;

	TArray<FVector> LineVertices;
	TArray<int32> LineTriangles;

	//horizontal line geometry
	for (size_t i = 0; i < NumRows+1; i++)
	{
		Y_i = i * TileSize;
		LineEnd = GridWidth();

		CreateLine(FVector(0.0f, Y_i, 0.0f), FVector(LineEnd, Y_i, 0.0f), LineThickness, LineVertices, LineTriangles);
	}

	//vertical line geometry
	for (size_t i = 0; i < NumColumns+1; i++)
	{
		X_i = i * TileSize;
		LineEnd = GridHight();

		CreateLine(FVector(X_i, 0.0f, 0.0f), FVector(X_i, LineEnd, 0.0f), LineThickness, LineVertices, LineTriangles);
	}

	TArray<FVector> normals;
	TArray<FVector2D> UV0;
	TArray<FProcMeshTangent> tangents;
	TArray<FLinearColor> vertexColors;

	GridMesh->CreateMeshSection_LinearColor(0, LineVertices, LineTriangles, normals, UV0, vertexColors, tangents, false);
}

void AGrid_Actor::DrawSelection()
{
	float Y = TileSize / 2.0f;

	CreateLine(FVector(0.0f, Y, 0.0f), FVector(TileSize, Y, 0.0f), TileSize, SelectionVertices, SelectionTriangles);

	TArray<FVector> normals;
	TArray<FVector2D> UV0;
	TArray<FProcMeshTangent> tangents;
	TArray<FLinearColor> vertexColors;

	SelectionMesh->CreateMeshSection_LinearColor(0, SelectionVertices, SelectionTriangles, normals, UV0, vertexColors, tangents, false);

	SelectionVertices = {};
	SelectionTriangles = {};
}


float AGrid_Actor::GridWidth() const
{
	return NumColumns * TileSize;
}

float AGrid_Actor::GridHight() const
{
	return NumRows * TileSize;
}

UMaterialInstanceDynamic* AGrid_Actor::CreateMaterialInstance(const FLinearColor Color, const float Opacity, UProceduralMeshComponent* Mesh)
{
	UMaterialInstanceDynamic* LineMaterialInstance = nullptr;
	if (Mesh)
	{
		LineMaterialInstance = Mesh->CreateDynamicMaterialInstance(0, Material);
		LineMaterialInstance->SetVectorParameterValue(FName("Color"), Color);
		LineMaterialInstance->SetScalarParameterValue(FName("Opacity"), Opacity);
		
	}
	
	if (LineMaterialInstance == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Material Instance Could not be created"));
	}

	return LineMaterialInstance;
}


bool AGrid_Actor::LocationToTile(const FVector& Location, int& Row, int& Column)
{
	return false;
}

void AGrid_Actor::TileToGrid(const int& Row, const int& Column, const bool Center, bool& Valid, FVector2D& GridLocation)
{
}

void AGrid_Actor::SetSelectedTile(const int& Row, const int& Column)
{
}

