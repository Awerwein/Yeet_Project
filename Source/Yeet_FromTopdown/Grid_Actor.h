// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Math/Color.h"
#include "Math/Vector.h"
#include "Math/Vector2D.h"
#include "Materials/MaterialInterface.h"
#include "ProceduralMeshComponent.h"

#include "Grid_Actor.generated.h"



class UMaterialInterface;

UCLASS()
class YEET_FROMTOPDOWN_API AGrid_Actor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGrid_Actor();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int32 NumRows;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int32 NumColumns;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float TileSize;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float LineThickness;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FLinearColor LineColor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FLinearColor SelectionColor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float LineOpacity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float SelectionOpacity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UMaterialInterface* Material;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		USceneComponent* RootSceneComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UProceduralMeshComponent* GridMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UProceduralMeshComponent* SelectionMesh;

	UPROPERTY(EditAnywhere)
		TArray<FVector> SelectionVertices;

	UPROPERTY(EditAnywhere)
		TArray<int32> SelectionTriangles;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	UFUNCTION(BlueprintCallable)
		virtual void CreateLine(const FVector Start, const FVector End, const float& Thickness, TArray<FVector>& Vertices, TArray<int32>& Triangles);

	UFUNCTION(BlueprintCallable)
		virtual void CreateCorner(const FVector Start, const float& Thickness, TArray<FVector>& Vertices, TArray<int32>& Triangles);

	UFUNCTION(BlueprintCallable)
		virtual void DrawGrid();

	UFUNCTION(BlueprintPure)
		float GridWidth() const;

	UFUNCTION(BlueprintPure)
		float GridHight() const;

	UFUNCTION(BlueprintCallable)
		virtual UMaterialInstanceDynamic* CreateMaterialInstance(const FLinearColor Color, const float Opacity, UProceduralMeshComponent* Mesh);

	UFUNCTION(BlueprintCallable)
		virtual void DrawSelection();

public:
	UFUNCTION(BlueprintCallable)
		virtual bool LocationToTile(const FVector& Location, int& Row, int& Column);

	UFUNCTION(BlueprintCallable)
		virtual void TileToGrid(const int& Row, const int& Column, const bool Center, bool& Valid, FVector2D& GridLocation);

	UFUNCTION(BlueprintCallable)
		virtual void SetSelectedTile(const int& Row, const int& Column);
};
