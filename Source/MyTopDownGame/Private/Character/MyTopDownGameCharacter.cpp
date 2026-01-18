#include "Character/MyTopDownGameCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Character/BossCharacter.h"
#include "UI/RhythmWidgetBase.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Misc/OutputDeviceNull.h"

AMyTopDownGameCharacter::AMyTopDownGameCharacter()
{
    PrimaryActorTick.bCanEverTick = true; // 必须开启 Tick 以更新转向

    // 2.5D 常用配置：禁用角色随运动方向自动转向
    bUseControllerRotationPitch = false;
    bUseControllerRotationRoll = false;
    bUseControllerRotationYaw = false;

    if (GetCharacterMovement())
    {
        GetCharacterMovement()->bOrientRotationToMovement = false; // 不随移动转向
        GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
    }
}

void AMyTopDownGameCharacter::BeginPlay()
{
    Super::BeginPlay();

    APlayerController* PC = Cast<APlayerController>(GetController());
    if (PC)
    {
        // 1. 强制显示鼠标指针
        PC->bShowMouseCursor = true;
        PC->bEnableClickEvents = true;
        PC->bEnableMouseOverEvents = true;

        // 2. 优化输入模式设置
        FInputModeGameAndUI InputMode; // 改为 GameAndUI 模式更稳妥
        InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
        InputMode.SetHideCursorDuringCapture(false); // 关键：抓取时不隐藏光标
        PC->SetInputMode(InputMode);
        FSlateApplication::Get().SetAllUserFocusToGameViewport();
        
        // 激活 IMC 逻辑保持不变...
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
        {
            if (DefaultMappingContext)
            {
                Subsystem->AddMappingContext(DefaultMappingContext, 0);
            }
        }
    }

    ScheduleNextRhythm();
}

void AMyTopDownGameCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // 每一帧更新朝向鼠标
    UpdateLookRotation(DeltaTime);
}

void AMyTopDownGameCharacter::UpdateLookRotation(float DeltaTime)
{
    APlayerController* PC = Cast<APlayerController>(GetController());
    if (PC)
    {
        FHitResult TraceHitResult;
        // 获取鼠标光标下的世界位置（检测 Visibility 频道）
        PC->GetHitResultUnderCursor(ECC_Visibility, false, TraceHitResult);
        
        if (TraceHitResult.bBlockingHit)
        {
            FVector TargetLocation = TraceHitResult.ImpactPoint;
            FVector CharacterLocation = GetActorLocation();
            
            // 计算方向：目标位置 - 自身位置
            FVector LookDirection = TargetLocation - CharacterLocation;
            LookDirection.Z = 0.f; // 锁定高度，防止角色倾斜

            if (!LookDirection.IsNearlyZero())
            {
                // 将向量转为旋转量
                FRotator TargetRotation = LookDirection.Rotation();
                
                // 平滑插值：10.f 是转向速度，可以根据手感调整
                FRotator NewRotation = FMath::RInterpTo(GetActorRotation(), TargetRotation, DeltaTime, 10.f);
                SetActorRotation(NewRotation);
            }
        }
    }
}

void AMyTopDownGameCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
    {
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AMyTopDownGameCharacter::OnRhythmPressed);
    }
}

void AMyTopDownGameCharacter::OnRhythmPressed()
{
    AActor* FoundActor = UGameplayStatics::GetActorOfClass(GetWorld(), ABossCharacter::StaticClass());
    ABossCharacter* Boss = Cast<ABossCharacter>(FoundActor);

    if (RhythmUI && RhythmUI->bIsActive)
    {
        float Accuracy = RhythmUI->CurrentScale;

        if (Accuracy >= 0.9f && Accuracy <= 1.10f)
        {
            RhythmUI->PlayPerfectAnim(); 
            // 朝着角色当前正前方（即鼠标指向的方向）冲刺
            LaunchCharacter(GetActorForwardVector() * 800.f, true, false);
            if (Boss) Boss->AddAlert(-10.f);
        }
        else
        {
            RhythmUI->PlayMissAnim();
            if (Boss) Boss->AddAlert(34.0f);
        }

        RhythmUI->bIsActive = false; 
        ScheduleNextRhythm();
    }
}

// void AMyTopDownGameCharacter::ScheduleNextRhythm()
// {
//     GetWorldTimerManager().ClearTimer(RhythmTimerHandle);
//     float RandomDelay = FMath::RandRange(MinInterval, MaxInterval);
//     RandomDelay = FMath::Max(1.0f, RandomDelay);
//
//     GetWorldTimerManager().SetTimer(RhythmTimerHandle, this, &AMyTopDownGameCharacter::TriggerRandomRhythm, RandomDelay, false);
// }




void AMyTopDownGameCharacter::ScheduleNextRhythm()
{
    GetWorldTimerManager().ClearTimer(RhythmTimerHandle);

    float RandomDelay;

    // 1. 定义一个概率，比如 20% 的几率立刻触发（0.1秒）
    // 你也可以根据需要调整这个阈值
    if (FMath::FRand() < 0.5f) 
    {
        // 第一种情况：0.1秒后立刻触发
        RandomDelay = 0.3f;
    }
    else 
    {
        // 第二种情况：原来的随机逻辑
        RandomDelay = FMath::RandRange(MinInterval, MaxInterval);
        RandomDelay = FMath::Max(1.0f, RandomDelay);
    }

    // 2. 设置定时器
    GetWorldTimerManager().SetTimer(RhythmTimerHandle, this, &AMyTopDownGameCharacter::TriggerRandomRhythm, RandomDelay, false);
}

void AMyTopDownGameCharacter::TriggerRandomRhythm()
{
    if (RhythmUI)
    {
        RhythmUI->StartRhythm();
    }
}

void AMyTopDownGameCharacter::ShowGameResult(bool bIsWin)
{

    // 检查是否已经在编辑器里指派了 WBP_GameResult 类
    if (ResultWidgetClass)
    {
        UUserWidget* ResultInstance = CreateWidget<UUserWidget>(GetWorld(), ResultWidgetClass);
        if (ResultInstance)
        {
            ResultInstance->AddToViewport();

            // 1. 冻结游戏逻辑
            UGameplayStatics::SetGamePaused(GetWorld(), true);

            // 2. 转换输入模式，让鼠标能点击 UI 按钮
            APlayerController* PC = Cast<APlayerController>(GetController());
            if (PC)
            {
                FInputModeUIOnly InputMode;
                InputMode.SetWidgetToFocus(ResultInstance->TakeWidget());
                PC->SetInputMode(InputMode);
                PC->bShowMouseCursor = true;
            }

            // 3. 通知 UI 它是“胜利”还是“失败”
            // 这里调用你蓝图里的 UpdateResultView 事件
            FOutputDeviceNull ar;
            ResultInstance->CallFunctionByNameWithArguments(
                *FString::Printf(TEXT("UpdateResultView %s"), bIsWin ? TEXT("True") : TEXT("False")), 
                ar, NULL, true
            );
        }
    }
}
