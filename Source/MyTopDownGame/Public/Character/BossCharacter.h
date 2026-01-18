#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BossCharacter.generated.h"

// 声明委托，方便 UI 监听警戒值变化
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAlertChanged, float, NewAlertLevel);

// Boss 行为状态枚举
UENUM(BlueprintType)
enum class EBossState : uint8 {
	Idle        UMETA(DisplayName = "Home"),
	Observing   UMETA(DisplayName = "Observing"),
	Chasing     UMETA(DisplayName = "Chasing")
};

UCLASS()
class MYTOPDOWNGAME_API ABossCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABossCharacter();

	// --- 核心属性 ---
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss|Stats")
	float AlertLevel = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss|Stats")
	float MaxAlertLevel = 100.0f;

	// 警戒值自动下降的速度（每秒）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss|Stats")
	float CoolDownRate = 2.0f;



	FVector HomeLocation;

	// --- 状态与事件 ---
	UPROPERTY(BlueprintAssignable, Category = "Boss|Events")
	FOnAlertChanged OnAlertChanged;

	// 这里的 ObservationTarget 就是你地图里的 Target Point
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss AI")
	AActor* ObservationTarget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Boss|State")
	EBossState CurrentState = EBossState::Idle;

	// 修改警戒值的接口
	UFUNCTION(BlueprintCallable, Category = "Boss|Logic")
	void AddAlert(float Amount);

protected:

	UPROPERTY()
	class AMyTopDownGameCharacter* CachedPlayer;

	UPROPERTY()
	class AAIController* AIC;
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	// 决策与移动核心
	void UpdateBossBehavior();
};