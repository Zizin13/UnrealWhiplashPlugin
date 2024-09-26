#pragma once
//-------------------------------------------------------------------------------------------------
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "WhiplashLoader.generated.h"
//-------------------------------------------------------------------------------------------------

UENUM(BlueprintType)
enum class EWhipModel : uint8
{
  CAR_F1WACK = 0,
  CAR_XAUTO,
  CAR_XDESILVA,
  CAR_XPULSE,
  CAR_XGLOBAL,
  CAR_XMILLION,
  CAR_XMISSION,
  CAR_XZIZIN,
  CAR_XREISE,
  CAR_YAUTO,
  CAR_YDESILVA,
  CAR_YPULSE,
  CAR_YGLOBAL,
  CAR_YMILLION,
  CAR_YMISSION,
  CAR_YZIZIN,
  CAR_YREISE,
  CAR_DEATH,
  SIGN_TOWER,
  SIGN_TOWER2,
  SIGN_SIGN01,
  SIGN_SIGN02,
  SIGN_BUILD,
  SIGN_BUILD1,
  SIGN_BUILD2,
  SIGN_BUILD3,
  SIGN_HEELBAR,
  SIGN_BALLOON,
  SIGN_BALLOON2,
  SIGN_TREE,
  SIGN_ADVERT,
  SIGN_ADVERT2,
  SIGN_ADVERT3,
  SIGN_QUADBLD,
  SIGN_BLD0
};

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