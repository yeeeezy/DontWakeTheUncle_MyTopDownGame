#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TopDownProtoGameModeBase.generated.h"

// 前向声明，告诉编译器这个类存在，不需要现在包含它的头文件
class URhythmWidgetBase;

UCLASS()
class MYTOPDOWNGAME_API ATopDownProtoGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	/** 在编辑器里把 WBP_RhythmCircle 选到这个槽位里 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rhythm Game")
	TSubclassOf<URhythmWidgetBase> RhythmWidgetClass;

protected:
	/** 游戏开始时的初始化 */
	virtual void BeginPlay() override;
};