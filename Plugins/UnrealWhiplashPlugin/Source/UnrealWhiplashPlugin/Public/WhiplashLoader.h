#pragma once
//-------------------------------------------------------------------------------------------------
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "WhiplashTypes.h"
#include "WhiplashLoader.generated.h"
//-------------------------------------------------------------------------------------------------

USTRUCT(BlueprintType)
struct FWhipModel
{
  GENERATED_USTRUCT_BODY()

  UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ModelData")
  TArray<FVector> Vertices;

  UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ModelData")
  TArray<int32> Indices;

  UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ModelData")
  TArray<FVector> Normals;

  UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ModelData")
  TArray<FVector2D> TexCoords;

  UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ModelData")
  UTexture2D *Texture;
};

//-------------------------------------------------------------------------------------------------

UCLASS()
class UNREALWHIPLASHPLUGIN_API UWhiplashLoader : public UBlueprintFunctionLibrary
{
  GENERATED_BODY()

  UFUNCTION(BlueprintCallable, Category = "WhiplashLoader")
  static UTexture2D* LoadTexture(const FString& TexFile, const FString &PalFile, bool& IsValid);

  UFUNCTION(BlueprintCallable, Category = "WhiplashLoader")
  static FWhipModel LoadModel(EWhipModel model, const FString &TexFile, const FString &PalFile, bool &IsValid);
};

//-------------------------------------------------------------------------------------------------