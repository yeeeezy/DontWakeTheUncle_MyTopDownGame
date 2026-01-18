#include "Character/BossCharacter.h"
#include "AIController.h"
#include "Character/MyTopDownGameCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

ABossCharacter::ABossCharacter()
{
    PrimaryActorTick.bCanEverTick = true;
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}



void ABossCharacter::BeginPlay()
{
    Super::BeginPlay();
    
    // 1. 记录出生点
    HomeLocation = GetActorLocation();

    // 2. 初始化缓存：获取 AI 控制器
    AIC = Cast<AAIController>(GetController());

    // 3. 初始化缓存：获取并存储玩家引用（方法 B）
    CachedPlayer = Cast<AMyTopDownGameCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

void ABossCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // 每一帧检查是否抓到了玩家
    if (CurrentState == EBossState::Chasing && CachedPlayer)
    {
        float Distance = GetDistanceTo(CachedPlayer);
        
        // 如果距离小于 150，触发失败 UI
        if (Distance < 150.0f)
        {
            CachedPlayer->ShowGameResult(false);
        }
    }
}

void ABossCharacter::AddAlert(float Amount)
{
    float OldLevel = AlertLevel;
    AlertLevel = FMath::Clamp(AlertLevel + Amount, 0.0f, MaxAlertLevel);

    if (OldLevel != AlertLevel)
    {
        OnAlertChanged.Broadcast(AlertLevel);
        UpdateBossBehavior(); 
    }
}

void ABossCharacter::UpdateBossBehavior()
{
    if (!AIC || !CachedPlayer) return;

    // 状态 A：满警戒 (100) -> 追击模式
    if (AlertLevel >= 100.0f)
    {
        if (CurrentState != EBossState::Chasing)
        {
            CurrentState = EBossState::Chasing;
            GetCharacterMovement()->MaxWalkSpeed = 600.0f;
        }
        
        // 直接使用缓存的 CachedPlayer，不再使用 GetAllActorsOfClass
        AIC->MoveToActor(CachedPlayer, 50.0f);
    }
    
    // 状态 B：观察阶段 [30, 100)
    else if (AlertLevel >= 30.0f)
    {
        if (CurrentState != EBossState::Observing)
        {
            CurrentState = EBossState::Observing;
            GetCharacterMovement()->MaxWalkSpeed = 300.0f;

            if (ObservationTarget)
            {
                AIC->MoveToLocation(ObservationTarget->GetActorLocation());
            }
            else if (GEngine)
            {
                GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("警告：未设置 ObservationTarget!"));
            }
        }
    }
    
    // 状态 C：回家阶段 [< 20]
    else if (AlertLevel < 20.0f) 
    {
        if (CurrentState != EBossState::Idle)
        {
            CurrentState = EBossState::Idle;
            GetCharacterMovement()->MaxWalkSpeed = 150.0f;
            AIC->MoveToLocation(HomeLocation);
        }
    }
}