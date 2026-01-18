#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Border.h" // 必须包含 Border 头文件
#include "RhythmWidgetBase.generated.h"

UCLASS()
class MYTOPDOWNGAME_API URhythmWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:
	// 缩放增长速度：1.0 代表 1 秒从 0 涨到 1.0
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rhythm")
	float GrowthSpeed = 1.0f;

	// 当前缩放值，供 Character 类读取判断
	UPROPERTY(BlueprintReadOnly, Category = "Rhythm")
	float CurrentScale = 0.0f;

	// 标记当前节奏圆是否处于活动状态
	UPROPERTY(BlueprintReadWrite, Category = "Rhythm")
	bool bIsActive = false;

	UFUNCTION(BlueprintImplementableEvent)
	void PlayPerfectAnim();

	UFUNCTION(BlueprintImplementableEvent)
	void PlayMissAnim();


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rhythm|Audio")
	class USoundBase* PerfectSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rhythm|Audio")
	class USoundBase* MissSound;
	
protected:
	// 绑定蓝图中的名为 Border_Inner 的 Border 组件
	UPROPERTY(meta = (BindWidget))
	UBorder* Border_Inner;

	// 基础颜色，可以在蓝图细节面板直接修改
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rhythm|Visual")
	FLinearColor BaseColor = FLinearColor(0.202651f, 0.214889f, 1.0f, 1.0f);
	
	// 每一帧更新缩放数值和视觉表现
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	

public:
	// 启动节奏圆
	UFUNCTION(BlueprintCallable, Category = "Rhythm")
	void StartRhythm();

	// 重置/关闭节奏圆
	UFUNCTION(BlueprintCallable, Category = "Rhythm")
	void ResetRhythm();
};