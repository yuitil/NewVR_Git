//VRPawnCPP.cpp

#include "VRPawnCPP.h"
#include "Camera/CameraComponent.h"
#include "Components/SceneComponent.h"
#include "MotionControllerComponent.h"
#include "GameFramework/PlayerController.h"

#include "DrawDebugHelpers.h"//画面デバッグ用

AVRPawnCPP::AVRPawnCPP() : 
	fMoveSpeed(1200.f),
	fSwingThreshold(5.f),
	fForwardBlockedRay(70.f)
{
	PrimaryActorTick.bCanEverTick = true;

	//Root
	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	//カメラ
	pCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("VRCamera"));
	pCamera->SetupAttachment(RootComponent);

	//コントローラー(右)
	pMotionControllerRight = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MotionControllerRight"));
	pMotionControllerRight->SetupAttachment(RootComponent);
	pMotionControllerRight->SetTrackingSource(EControllerHand::Right);

	//コントローラー(左)
	pMotionControllerLeft = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MotionControllerLeft"));
	pMotionControllerLeft->SetupAttachment(RootComponent);
	pMotionControllerLeft->SetTrackingSource(EControllerHand::Left);
}

void AVRPawnCPP::BeginPlay()
{
	Super::BeginPlay();
	
	//右手の初期位置を記録
	vLastRightHandPos = pMotionControllerRight->GetComponentLocation();
}

void AVRPawnCPP::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//コントローラー振り移動処理
	HandleHandSwingMovement(DeltaTime);

	//右手位置を保存
	vLastRightHandPos = pMotionControllerRight->GetComponentLocation();
}

void AVRPawnCPP::HandleHandSwingMovement(float DeltaTime)
{
	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("YYYYYYYY! : [%f]")));
	
	//現在の右手位置
	FVector CurrentPos = pMotionControllerRight->GetComponentLocation();

	//移動量
	FVector vDelta = CurrentPos - vLastRightHandPos;

	//Z軸の大きさを取得
	float VerticalMove = vDelta.Z;

	//一定以上振り降ろしたか判定
	if (VerticalMove < -fSwingThreshold)
	{
		//前方に障害物があるかチェック
		if (IsForwardBlocked(fForwardBlockedRay))
		{
			return;
		}

		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("MMMMMMMMMMM! : [%f]")));

		//カメラの前方向を取得
		FVector vForward = pCamera->GetForwardVector();

		//水平移動だけにしたい場合はZ成分を消す
		vForward.Z = 0.f;
		vForward.Normalize();

		//Pawnを前進させる
		AddActorWorldOffset(vForward * fMoveSpeed * DeltaTime, true);
	}
}

bool AVRPawnCPP::IsForwardBlocked(float fDistance)
{
	//カメラの位置
	FVector vStart = pCamera->GetComponentLocation();
	//高さを下げる
	vStart.Z -= 65.f;
	//方向
	FVector vDirection = pCamera->GetForwardVector();

	//終了位置
	FVector vEnd = vStart + vDirection * fDistance;

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		Hit,
		vStart,
		vEnd,
		ECC_Visibility,  //レイのコリジョン
		Params
	);

	//デバッグ用のレイ（赤ヒット/緑ヒットなし）
	//FColor LineColor = bHit ? FColor::Red : FColor::Green;
	//DrawDebugLine(GetWorld(), vStart, vEnd, LineColor, false, 0.1f, 0, 2.f);

	return bHit;
}

