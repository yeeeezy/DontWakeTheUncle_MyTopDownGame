#include "Framework/TopDownProtoGameModeBase.h"
#include "UI/RhythmWidgetBase.h"      // 必须包含 UI 的头文件才能调用其函数
#include "Character/MyTopDownGameCharacter.h" // 必须包含角色头文件才能赋值
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

void ATopDownProtoGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	
	// 1. 检查是否在编辑器里设置了 UI 类
	if (RhythmWidgetClass)
	{
		// 2. 创建 UI 实例
		URhythmWidgetBase* RhythmUI = CreateWidget<URhythmWidgetBase>(GetWorld(), RhythmWidgetClass);
        
		if (RhythmUI)
		{
			// 3. 将 UI 添加到玩家屏幕
			RhythmUI->AddToViewport();

			// 4. 获取当前的玩家角色 (Player 0)
			APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
			AMyTopDownGameCharacter* MyChar = Cast<AMyTopDownGameCharacter>(PlayerPawn);

			if (MyChar)
			{
				// 5. 【核心步骤】将创建好的 UI 引用交给角色里的指针
				MyChar->RhythmUI = RhythmUI;
                
		
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("GameMode: Failed to cast Player Pawn to MyTopDownGameCharacter!"));
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("GameMode: RhythmWidgetClass is NOT assigned in Blueprint!"));
	}
}