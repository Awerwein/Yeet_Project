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

	FVector Zero = Start + (LineVector * HalfThickness) + (ThicknessDirection * HalfThickness);
	FVector Two = Start + (LineVector * HalfThickness) - (ThicknessDirection * HalfThickness);
	FVector One = End - (LineVector * HalfThickness) + (ThicknessDirection * HalfThickness);
	FVector Three = End - (LineVector * HalfThickness) - (ThicknessDirection * HalfThickness);

	Vertices += {Zero, One, Two, Three};


}

void AGrid_Actor::CreateCorner(const FVector Start, const float& Thickness, TArray<FVector>& Vertices, TArray<int32>& Triangles)
{
	float HalfThickness = Thickness / 2.0f;
	int32 Size = Vertices.Num();
	Triangles += {Size + 2, Size + 1, Size, Size + 2, Size + 3, Size + 1};

	FVector Zero = Start + FVector(1.0f * HalfThickness, 1.0f * HalfThickness, 0.0f);
	FVector Two = Start + FVector(1.0f * HalfThickness, -1.0f * HalfThickness, 0.0f);
	FVector One = Start + FVector(-1.0f * HalfThickness, 1.0f * HalfThickness, 0.0f);
	FVector Three = Start + FVector(-1.0f * HalfThickness, -1.0f * HalfThickness, 0.0f);

	Vertices += {Zero, One, Two, Three};

}

void AGrid_Actor::DrawGrid()
{


	float LineEnd = 0.0f;
	float X_i = 0.0f;
	float Y_i = 0.0f;
	float X_start = 0.0f;
	float X_end = 0.0f;
	float Y_start = 0.0f;
	float Y_end = 0.0f;


	TArray<FVector> LineVertices;
	TArray<int32> LineTriangles;

	//horizontal line geometry
	for (size_t i = 0; i < NumRows+1; i++)
	{
		Y_i = i * TileSize;
		LineEnd = GridWidth();
		for (size_t j = 0; j < NumColumns; j++)
		{
			X_start = j * TileSize;
			X_end = X_start + TileSize;
			CreateLine(FVector(X_start, Y_i, 0.0f), FVector(X_end, Y_i, 0.0f), LineThickness, LineVertices, LineTriangles);
		}

		
	}

	//vertical line geometry
	for (size_t i = 0; i < NumColumns+1; i++)
	{
		X_i = i * TileSize;
		LineEnd = GridHight();
		for (size_t j = 0; j < NumRows; j++)
		{
			Y_start = j * TileSize;
			Y_end = Y_start + TileSize;
			CreateLine(FVector(X_i, Y_start, 0.0f), FVector(X_i, Y_end, 0.0f), LineThickness, LineVertices, LineTriangles);
		}
		
	}

	//corner geometry
	//inefficient, but readable
	for (size_t i = 0; i < NumColumns+1; i++)
	{
		X_i = i * TileSize;
		for (size_t j = 0; j < NumRows+1; j++)
		{
			Y_i = j * TileSize; //should be Y_j all along
			CreateCorner(FVector(X_i, Y_i, 0.0f), LineThickness, LineVertices, LineTriangles);
		}
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
	Row = floor(((Location.X - GetActorLocation().X) / GridWidth()) * NumRows);
	Column = floor(((Location.Y - GetActorLocation().Y) / GridHight()) * NumRows);
	bool valid = ((Row >= 0) && Row < NumRows && Column >= 0 && Column < NumColumns);
	return valid;
}

void AGrid_Actor::TileToGrid(const int& Row, const int& Column, const bool Center, bool& Valid, FVector2D& GridLocation)
{
}

void AGrid_Actor::SetSelectedTile(const int& Row, const int& Column)
{
}

