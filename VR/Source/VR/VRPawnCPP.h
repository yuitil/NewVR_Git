//VRPawnCPP.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "VRPawnCPP.generated.h"

//前方宣言
class UCameraComponent;
class UMotionControllerComponent;

UCLASS()
class VR_API AVRPawnCPP : public APawn
{
	GENERATED_BODY()

public:
	AVRPawnCPP();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

public:	

	//VRカメラを割り当てる
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR")
	UCameraComponent* pCamera;

	//右手のモーションコントローラーを割り当てる
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR")
	UMotionControllerComponent* pMotionControllerRight;

	//左手のモーションコントローラーを割り当てる
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR")
	UMotionControllerComponent* pMotionControllerLeft;

	//コントローラーの振り移動の強さ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR|Mpvement")
	float fMoveSpeed;

	//最終フレームからの右手位置
	FVector vLastRightHandPos;

	//右手を縦に振ったかどうか判定するための値
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR|Mpvement")
	float fSwingThreshold;

	//移動先壁判定用のレイの長さ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR|Mpvement")
	float fForwardBlockedRay;

	//移動実行関数
	void HandleHandSwingMovement(float DeltaTime);

	//移動先に壁があるか判定する関数
	bool IsForwardBlocked(float DeltaTime);
};
