#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "UI/RhythmWidgetBase.h" // 必须引用你的 UI 类
#include "MyTopDownGameCharacter.generated.h"

UCLASS()
class MYTOPDOWNGAME_API AMyTopDownGameCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AMyTopDownGameCharacter();

	void ScheduleNextRhythm();

	// 显示结算界面的函数
	UFUNCTION(BlueprintCallable, Category = "UI")
	void ShowGameResult(bool bIsWin);
protected:

	
	virtual void BeginPlay() override;
	void Tick(float DeltaTime);
	void UpdateLookRotation(float DeltaTime);
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// 在类定义中
	// 在 MyTopDownGameCharacter.h 中
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputMappingContext* DefaultMappingContext;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* JumpAction; // 用于在编辑器里关联 IA_Jump
	
	// 处理节奏按键
	void OnRhythmPressed();

	// 处理随机触发的函数
	void TriggerRandomRhythm();

	// 计时器句柄
	FTimerHandle RhythmTimerHandle;

	// 随机触发的时间范围
	UPROPERTY(EditAnywhere, Category = "Rhythm")
	float MinInterval = 3.0f;

	UPROPERTY(EditAnywhere, Category = "Rhythm")
	float MaxInterval = 7.0f;

	// 在编辑器里指派你的 WBP_GameResult 蓝图类
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UUserWidget> ResultWidgetClass;



public:
	// 指向 UI 的指针（需要在蓝图里给它赋值）
	UPROPERTY(BlueprintReadWrite, Category = "Rhythm")
	URhythmWidgetBase* RhythmUI;


};