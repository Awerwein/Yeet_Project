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
//#pragma optimize( "", off )
void AGrid_Actor::DrawGrid()
{


	float LineEnd = 0.0f;
	float X_i = 0.0f;
	float Y_i = 0.0f;
	float X_start = 0.0f;
	float X_end = 0.0f;
	float Y_start = 0.0f;
	float Y_end = 0.0f;

	int32 Size = 0;

	TArray<FVector> LineVertices;
	TArray<int32> LineTriangles;
	//Problem here, array length calculated once, when actor is created
	TArray<int32> ControlVectorIndices;
	for (size_t i = 0; i < 4 * NumRows * NumColumns*10; i++)
	{
		ControlVectorIndices += {0};
	}

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
	int index_a = 0;
	int index_b = 0;

	for (size_t j = 0; j < NumRows+1; j++)
	{
		Y_i = j * TileSize; //should be Y_j all along

		for (size_t i = 0; i < NumColumns + 1; i++)
		{
			X_i = i * TileSize;
			CreateCorner(FVector(X_i, Y_i, 0.0f), LineThickness, LineVertices, LineTriangles);
			//corners for nurbs
			Size = LineVertices.Num();
			index_a = 4 * NumColumns * j + 4 * i;
			index_b = 4 * NumRows * (j-1) + 4 * i;
			//zero
			if (i< NumColumns && j < NumRows)
			{
				ControlVectorIndices[index_a] = Size - 3;
			}
			//one
			if (i > 0  && j < NumRows)
			{
				ControlVectorIndices[index_a-3] = Size - 2;
			}
			//two
			if (i < NumColumns && j > 0)
			{
				ControlVectorIndices[index_b+3] = Size - 1;
			}
			//three
			if (i > 0 && j > 0)
			{
				ControlVectorIndices[index_b-2] = Size;
			}


		}
	}


	//nurbs
	//construct knot vector U
	float pi = 3.1415926535897932384626433832795028841971693993751058209749445923078164062;//no idea how the constant is supposed to work in ue4, quick hack
	TArray<float> KnotVector = {0.0f, 0.0f, 0.0f, 0.5f*pi, 0.5f*pi, 1.0f * pi, 1.0f * pi, 1.5f * pi, 1.5f * pi, 2.0f * pi, 2.0f * pi, 2.0f * pi };

	//construct control points c_i
	TArray<FVector> ControllPoints = {};
	int t = 1; //2^t subdivisions

	//for (size_t i = 0; i < 4; i += 4)
	//{
	//	ControllPoints += {LineVertices[ControlVectorIndices[i]]};
	//	for (size_t j = 1; j < 3; j++)
	//	{
	//		ControllPoints += {LineVertices[ControlVectorIndices[i]] + (LineVertices[ControlVectorIndices[i + 1]] - LineVertices[ControlVectorIndices[i]]) * j / (2^t)};
	//	}
	//	for (size_t k = 1; k < 3; k++)
	//	{
	//		ControllPoints += {LineVertices[ControlVectorIndices[i + 1]] + (LineVertices[ControlVectorIndices[i + 2]] - LineVertices[ControlVectorIndices[i + 1]]) * k / (2 ^ t)};
	//	}
	//	for (size_t l = 1; l < 3; l++)
	//	{
	//		ControllPoints += {LineVertices[ControlVectorIndices[i + 2]] + (LineVertices[ControlVectorIndices[i + 3]] - LineVertices[ControlVectorIndices[i + 2]]) * l / (2 ^ t)};
	//	}
	//	for (size_t m = 1; m < 3; m++)
	//	{
	//		ControllPoints += {LineVertices[ControlVectorIndices[i + 3]] + (LineVertices[ControlVectorIndices[i + 0]] - LineVertices[ControlVectorIndices[i + 3]]) * m / (2 ^ t)};
	//	}
	//}

	//test
	ControllPoints += {LineVertices[ControlVectorIndices[0]]};
	ControllPoints += {LineVertices[ControlVectorIndices[0]] + (LineVertices[ControlVectorIndices[1]] - LineVertices[ControlVectorIndices[0]]) /2};
	ControllPoints += {LineVertices[ControlVectorIndices[1]]};
	ControllPoints += {LineVertices[ControlVectorIndices[1]] + (LineVertices[ControlVectorIndices[2]] - LineVertices[ControlVectorIndices[1]]) / 2};
	ControllPoints += {LineVertices[ControlVectorIndices[2]]};
	ControllPoints += {LineVertices[ControlVectorIndices[2]] + (LineVertices[ControlVectorIndices[3]] - LineVertices[ControlVectorIndices[2]]) / 2};
	ControllPoints += {LineVertices[ControlVectorIndices[3]]};
	ControllPoints += {LineVertices[ControlVectorIndices[3]] + (LineVertices[ControlVectorIndices[0]] - LineVertices[ControlVectorIndices[3]]) / 2};
	ControllPoints += {LineVertices[ControlVectorIndices[0]]};

	//for (size_t i = 0; i < ControlVectorIndices.Num(); i+=4)
	//{
	//	ControllPoints += {LineVertices[ControlVectorIndices[i]]};
	//	for (size_t j = 1; j < t+1; j++)
	//	{
	//		ControllPoints += {LineVertices[ControlVectorIndices[i]] + (LineVertices[ControlVectorIndices[i + 1]] - LineVertices[ControlVectorIndices[i]])* j / t};
	//	}
	//	for (size_t k = 1; k < t+1; k++)
	//	{
	//		ControllPoints += {LineVertices[ControlVectorIndices[i+1]] + (LineVertices[ControlVectorIndices[i + 2]] - LineVertices[ControlVectorIndices[i+1]]) * k / t};
	//	}
	//	for (size_t l = 1; l < t + 1; l++)
	//	{
	//		ControllPoints += {LineVertices[ControlVectorIndices[i + 2]] + (LineVertices[ControlVectorIndices[i + 3]] - LineVertices[ControlVectorIndices[i + 2]]) * l / t};
	//	}
	//	for (size_t m = 1; m < t + 1; m++)
	//	{
	//		ControllPoints += {LineVertices[ControlVectorIndices[i + 3]] + (LineVertices[ControlVectorIndices[i + 0]] - LineVertices[ControlVectorIndices[i + 3]]) * m / t};
	//	}
	//
	//}

	//construct weights w_i
	/*TArray<float> Weights = { 1.0f };*/
	TArray<float> Weights = { 1.0f, 1.0f/sqrt(2.0f), 1.0f, 1.0f / sqrt(2.0f), 1.0f, 1.0f / sqrt(2.0f), 1.0f, 1.0f / sqrt(2.0f), 1.0f};

	TArray<FVector> NurbsVertices = {};
	CreateNurbsPoints(2, KnotVector, ControllPoints, NurbsVertices, Weights);


	for (size_t i = 0; i < NurbsVertices.Num(); i++)
	{
		//CreateCorner(NurbsVertices[i], 1.0f, LineVertices, LineTriangles);

	}

	TArray<FVector> normals;
	TArray<FVector2D> UV0;
	TArray<FProcMeshTangent> tangents;
	TArray<FLinearColor> vertexColors;

	GridMesh->CreateMeshSection_LinearColor(0, LineVertices, LineTriangles, normals, UV0, vertexColors, tangents, false);
}
//#pragma optimize( "", on )
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

float AGrid_Actor::BaseFuction(int i, int N, float u, const TArray<float>& KnotVector)
{
	//base fuction for B-Splines
	// 0 <= i <= M
	// N <= M
	// z dim is disregarded
	// int M = ControllPoints.Num() - 1;
	float out = -1.0f;

	if (N == 0)
	{
		if (KnotVector[i] <= u && u < KnotVector[i+1])
		{
			out = 1.0f;
		}
		else
		{
			out = 0.0f;
		}
	}
	else
	{
		if (KnotVector[i+N] != KnotVector[i] && KnotVector[i + N + 1] != KnotVector[i + 1])
		{
			out = ((u - KnotVector[i]) / (KnotVector[i + N] - KnotVector[i])) * BaseFuction(i, N-1, u, KnotVector) + (( KnotVector[i+ 1+ N] - u) / (KnotVector[i + 1 + N] - KnotVector[i+1])) * BaseFuction(i+1, N - 1, u, KnotVector);
		}
		else if (KnotVector[i + N] != KnotVector[i] && KnotVector[i + N + 1] == KnotVector[i + 1])
		{
			out = ((u - KnotVector[i]) / (KnotVector[i + N] - KnotVector[i])) * BaseFuction(i, N - 1, u, KnotVector);
		}
		else if (KnotVector[i + N] == KnotVector[i] && KnotVector[i + N + 1] != KnotVector[i + 1])
		{
			out = ((KnotVector[i + 1 + N] - u) / (KnotVector[i + 1 + N] - KnotVector[i + 1])) * BaseFuction(i + 1, N - 1, u, KnotVector);
		}
		else
		{
			out = 0.0f;
		}
	}
	return out;
}

void AGrid_Actor::CreateNurbsPoints(int N, const TArray<float>& KnotVector, const TArray<FVector>& ControllPoints, TArray<FVector>& NurbsVertices, const TArray<float>& Weights)
{
	//4*2^t subdivisions u, t later parameter, for now t=1
	int t = 1;
	int subs = 4 * (2 ^ t);
	float len = KnotVector[KnotVector.Num()-1] / subs;
	float len_counter = 0.0f; //u
	//fill vector array with new vertices x(u)
	int M = Weights.Num();

	for (size_t k = 0; k < subs; k++)
	{
		float R_sum = 0.0f;
		FVector c_sum = FVector(0.0f, 0.0f, 0.0f);
		for (size_t i = 0; i < M; i++)
		{
			float R_i = Weights[i] * BaseFuction(i, N, len_counter, KnotVector);
			c_sum.X += R_i * ControllPoints[i].X;
			c_sum.Y += R_i * ControllPoints[i].Y;
			R_sum += R_i;
		}
		c_sum = c_sum / R_sum;
		NurbsVertices += {c_sum};
	}

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
