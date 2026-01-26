#include "UI/RhythmWidgetBase.h"

#include "Character/BossCharacter.h"
#include "Character/MyTopDownGameCharacter.h"
#include "Kismet/GameplayStatics.h"

void URhythmWidgetBase::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	AActor* FoundActor = UGameplayStatics::GetActorOfClass(GetWorld(), ABossCharacter::StaticClass());
	ABossCharacter* Boss = Cast<ABossCharacter>(FoundActor);
	
	if (bIsActive && Border_Inner)
	{
		if (bIsActive && FMath::FRand() < 0.4f) // 1% 的几率在这一帧开始晃动
		{
			APlayerController* PC = GetOwningPlayer();
			if (PC && RhythmCameraShakeClass)
			{
				PC->ClientStartCameraShake(RhythmCameraShakeClass, 1.0f);
			}
		}// 累加缩放值
		CurrentScale += GrowthSpeed * InDeltaTime;

		// 直接设置 Border 的渲染缩放
		Border_Inner->SetRenderScale(FVector2D(CurrentScale, CurrentScale));


		// 如果超过 1.3 倍还没按，视为 Miss 并自动重置
		if (CurrentScale > 1.3f)
		{
			UE_LOG(LogTemp, Error, TEXT("Miss!"));
			if (Boss) Boss->AddAlert(34.0f);
			PlayMissAnim();
			ResetRhythm();
			
		}
	}
}

void URhythmWidgetBase::StartRhythm()
{
	bIsActive = true;
	CurrentScale = 0.0f;

	// --- 新增：随机偏移逻辑 ---
	// 假设你想在 X 和 Y 轴上产生 -50 到 50 像素之间的随机偏移
	float RandomX = FMath::RandRange(-200.0f, 200.0f);
	float RandomY = FMath::RandRange(-100.0f, 100.0f);
    
	// 设置整个 Widget 的渲染平移 (Render Translation)
	SetRenderTranslation(FVector2D(RandomX, RandomY));
	if (Border_Inner)
	{
		Border_Inner->SetVisibility(ESlateVisibility::Visible);
		Border_Inner->SetRenderScale(FVector2D(0.f, 0.f));
        
		// --- 新增：应用你定义的颜色变量 ---
		// 注意：Image 组件通常使用 SetBrushColor，Border 组件通常使用 SetContentColorAndOpacity
		Border_Inner->SetBrushColor(BaseColor); 
        
		// 确保透明度也是满的
		Border_Inner->SetRenderOpacity(1.0f);
	}
}

void URhythmWidgetBase::ResetRhythm()
{
	bIsActive = false;
	CurrentScale = 0.0f;


	AMyTopDownGameCharacter* MyChar = Cast<AMyTopDownGameCharacter>(GetOwningPlayerPawn());
	if (MyChar)
	{
		MyChar->ScheduleNextRhythm(); // “不按”也能续期
		UE_LOG(LogTemp, Warning, TEXT("Timeout! Auto-scheduling next..."));
	}
}