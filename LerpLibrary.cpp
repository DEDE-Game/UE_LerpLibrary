#pragma once
#include "LerpLibrary.h"
#include "Kismet/BlueprintFunctionLibrary.h"


#include "Engine/World.h"
#include "Camera/CameraComponent.h"
#include "TimerManager.h"

void ULerpLibrary::MoveActorToLocation(AActor* Actor, FVector TargetLocation, float Duration)
{
    if (Actor == nullptr || Duration <= 0.0f)
    {
        return;
    }
    FVector StartLocation = Actor->GetActorLocation();
    InitializeMove(Actor, Actor, StartLocation, TargetLocation, Duration);
}

void ULerpLibrary::RotateActorToRotation(AActor* Actor, FRotator TargetRotation, float Duration)
{
    if (Actor == nullptr || Duration <= 0.0f)
    {
        return;
    }

    FRotator StartRotation = Actor->GetActorRotation();
    InitializeRotate(Actor, Actor, StartRotation, TargetRotation, Duration);
}

void ULerpLibrary::ScaleActorToScale(AActor* Actor, FVector TargetScale, float Duration)
{
    if (Actor == nullptr || Duration <= 0.0f)
    {
        return;
    }
    FVector StartScale = Actor->GetActorScale3D();
    InitializeScale(Actor, Actor, StartScale, TargetScale, Duration);
}

void ULerpLibrary::InitializeMove(UObject* WorldContextObject, AActor* Actor, FVector StartLocation, FVector TargetLocation, float Duration)
{
    ULerpLibrary* LerpLibrary = NewObject<ULerpLibrary>(WorldContextObject);
    LerpLibrary->Actor = Actor;
    LerpLibrary->StartLocation = StartLocation;
    LerpLibrary->TargetLocation = TargetLocation;
    LerpLibrary->Duration = Duration;
    LerpLibrary->ElapsedTime = 0.0f;

    if (Actor->GetWorld())
    {
        FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(LerpLibrary, &ULerpLibrary::MoveActor, 0.01f);
        Actor->GetWorld()->GetTimerManager().SetTimer(LerpLibrary->TimerHandle, TimerDelegate, 0.01f, true);
    }
}

void ULerpLibrary::InitializeRotate(UObject* WorldContextObject, AActor* Actor, FRotator StartRotation, FRotator TargetRotation, float Duration)
{
    ULerpLibrary* LerpLibrary = NewObject<ULerpLibrary>(WorldContextObject);
    LerpLibrary->Actor = Actor;
    LerpLibrary->StartRotation = StartRotation;
    LerpLibrary->TargetRotation = TargetRotation;
    LerpLibrary->Duration = Duration;
    LerpLibrary->ElapsedTime = 0.0f;

    if (Actor->GetWorld())
    {
        FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(LerpLibrary, &ULerpLibrary::RotateActor, 0.01f);
        Actor->GetWorld()->GetTimerManager().SetTimer(LerpLibrary->TimerHandle, TimerDelegate, 0.01f, true);
    }
}

void ULerpLibrary::InitializeScale(UObject* WorldContextObject, AActor* Actor, FVector StartScale, FVector TargetScale, float Duration)
{
    ULerpLibrary* LerpLibrary = NewObject<ULerpLibrary>(WorldContextObject);
    LerpLibrary->Actor = Actor;
    LerpLibrary->StartScale = StartScale;
    LerpLibrary->TargetScale = TargetScale;
    LerpLibrary->Duration = Duration;
    LerpLibrary->ElapsedTime = 0.0f;

    if (Actor->GetWorld())
    {
        FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(LerpLibrary, &ULerpLibrary::ScaleActor, 0.01f);
        Actor->GetWorld()->GetTimerManager().SetTimer(LerpLibrary->TimerHandle, TimerDelegate, 0.01f, true);
    }
}

void ULerpLibrary::MoveActor(float DeltaTime)
{
    if (!Actor)
    {
        return;
    }

    ElapsedTime += DeltaTime;
    float Alpha = FMath::Clamp(ElapsedTime / Duration, 0.0f, 1.0f);

    FVector NewLocation = FMath::Lerp(StartLocation, TargetLocation, Alpha);
    Actor->SetActorLocation(NewLocation);

    if (Alpha >= 1.0f)
    {
        if (Actor->GetWorld())
        {
            Actor->GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
        }
    }
}



// Implementation of the static function to start lerp
void ULerpLibrary::LerpFloatToTarget(UObject* WorldContextObject, float& CurrentValue, float TargetValue, float Duration)
{
    if (WorldContextObject == nullptr || Duration <= 0.0f)
    {
        return;
    }

    // Create and initialize the lerp instance
    ULerpLibrary* LerpLibrary = NewObject<ULerpLibrary>(WorldContextObject);
    LerpLibrary->ValuePtr = &CurrentValue;
    LerpLibrary->TargetValue = TargetValue;
    LerpLibrary->Duration = Duration;
    LerpLibrary->ElapsedTime = 0.0f;

    if (WorldContextObject->GetWorld())
    {
        // Set up a timer to call LerpFloat regularly
        FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(LerpLibrary, &ULerpLibrary::LerpFloat, 0.01f);
        WorldContextObject->GetWorld()->GetTimerManager().SetTimer(LerpLibrary->LerpFloatTimerHandle, TimerDelegate, 0.01f, true);
    }
}

void ULerpLibrary::RotateActor(float DeltaTime)
{
    if (!Actor)
    {
        return;
    }

    ElapsedTime += DeltaTime;
    float Alpha = FMath::Clamp(ElapsedTime / Duration, 0.0f, 1.0f);

    FRotator NewRotation = FMath::Lerp(StartRotation, TargetRotation, Alpha);
    Actor->SetActorRotation(NewRotation);

    if (Alpha >= 1.0f)
    {
        if (Actor->GetWorld())
        {
            Actor->GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
        }
    }
}

void ULerpLibrary::ScaleActor(float DeltaTime)
{
    if (!Actor)
    {
        return;
    }

    ElapsedTime += DeltaTime;
    float Alpha = FMath::Clamp(ElapsedTime / Duration, 0.0f, 1.0f);

    FVector NewScale = FMath::Lerp(StartScale, TargetScale, Alpha);
    Actor->SetActorScale3D(NewScale);

    if (Alpha >= 1.0f)
    {
        if (Actor->GetWorld())
        {
            Actor->GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
        }
    }
}

void ULerpLibrary::MoveComponentToLocation(USceneComponent* Component, FVector TargetLocation, float Duration)
{
    if (Component == nullptr || Duration <= 0.0f)
    {
        return;
    }

    FVector StartLocation = Component->GetComponentLocation();
    InitializeMoveComponent(Component->GetOwner(), Component, StartLocation, TargetLocation, Duration);
}


void ULerpLibrary::MoveComponentToDynamicLocation(USceneComponent* ParentComponent, USceneComponent* ComponentToMove, FName SocketName, float Duration)
{
    if (!ParentComponent || !ComponentToMove || Duration <= 0.0f)
    {
        return;
    }

    // 初始化位置
    FVector StartLocation = ComponentToMove->GetComponentLocation();
    FVector TargetLocation = ParentComponent->GetSocketLocation(SocketName);

    // 初始化移动逻辑
    InitializeDynamicMoveComponent(
        ParentComponent->GetOwner(),
        ParentComponent,
        ComponentToMove,
        StartLocation,
        TargetLocation,
        SocketName,
        Duration);
}

void ULerpLibrary::InitializeDynamicMoveComponent(
    UObject* WorldContextObject,
    USceneComponent* ParentComponent,
    USceneComponent* ComponentToMove,
    FVector StartLocation,
    FVector TargetLocation,
    FName SocketName,
    float Duration)
{
    ULerpLibrary* LerpLibrary = NewObject<ULerpLibrary>(WorldContextObject);
    LerpLibrary->Component = ComponentToMove;
    LerpLibrary->ParentComponent = ParentComponent;
    LerpLibrary->SocketName = SocketName;
    LerpLibrary->StartLocation = StartLocation;
    LerpLibrary->TargetLocation = TargetLocation;
    LerpLibrary->Duration = Duration;
    LerpLibrary->ElapsedTime = 0.0f;

    if (ParentComponent->GetWorld())
    {
        FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(LerpLibrary, &ULerpLibrary::MoveComponentToDynamicLocationUpdate);
        ParentComponent->GetWorld()->GetTimerManager().SetTimer(LerpLibrary->TimerHandle, TimerDelegate, 0.01f, true);
    }
}

void ULerpLibrary::MoveComponentToDynamicLocationUpdate()
{
    if (!Component || !ParentComponent)
    {
        return;
    }

    // 更新目标位置
    TargetLocation = ParentComponent->GetSocketLocation(SocketName);

    // 计算DeltaTime（可以通过Timer间隔固定）
    float DeltaTime = 0.01f;

    // 更新时间与插值因子
    ElapsedTime += DeltaTime;
    float Alpha = FMath::Clamp(ElapsedTime / Duration, 0.0f, 1.0f);

    // 平滑更新位置
    FVector NewLocation = FMath::Lerp(StartLocation, TargetLocation, Alpha);
    Component->SetWorldLocation(NewLocation);

    // 判断是否到达目标
    if (Alpha >= 1.0f || FVector::Dist(NewLocation, TargetLocation) <= KINDA_SMALL_NUMBER)
    {
        if (Component->GetWorld())
        {
            Component->GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
        }
    }
}




void ULerpLibrary::RotateComponentToRotation(USceneComponent* Component, FRotator TargetRotation, float Duration)
{
    if (Component == nullptr || Duration <= 0.0f)
    {
        return;
    }

    FRotator StartRotation = Component->GetComponentRotation();
    InitializeRotateComponent(Component->GetOwner(), Component, StartRotation, TargetRotation, Duration);
}

void ULerpLibrary::RotateComponentToRelativeRotation(USceneComponent* Component, FRotator TargetRotation, float Duration)
{
    if (Component == nullptr || Duration <= 0.0f)
    {
        return;
    }
    InitializeRelativeRotateComponent(Component->GetOwner(), Component, TargetRotation, Duration);
}

void ULerpLibrary::InitializeMoveComponent(UObject* WorldContextObject, USceneComponent* Component, FVector StartLocation, FVector TargetLocation, float Duration)
{
    ULerpLibrary* LerpLibrary = NewObject<ULerpLibrary>(WorldContextObject);
    LerpLibrary->Component = Component;
    LerpLibrary->StartLocation = StartLocation;
    LerpLibrary->TargetLocation = TargetLocation;
    LerpLibrary->Duration = Duration;
    LerpLibrary->ElapsedTime = 0.0f;

    if (Component->GetWorld())
    {
        FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(LerpLibrary, &ULerpLibrary::MoveComponent, 0.01f);
        Component->GetWorld()->GetTimerManager().SetTimer(LerpLibrary->TimerHandle, TimerDelegate, 0.01f, true);
    }
}

void ULerpLibrary::InitializeRotateComponent(UObject* WorldContextObject, USceneComponent* Component, FRotator StartRotation, FRotator TargetRotation, float Duration)
{
    ULerpLibrary* LerpLibrary = NewObject<ULerpLibrary>(WorldContextObject);
    LerpLibrary->Component = Component;
    LerpLibrary->StartRotation = StartRotation;
    LerpLibrary->TargetRotation = TargetRotation;
    LerpLibrary->Duration = Duration;
    LerpLibrary->ElapsedTime = 0.0f;

    if (Component->GetWorld())
    {
        FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(LerpLibrary, &ULerpLibrary::RotateComponent, 0.01f);
        Component->GetWorld()->GetTimerManager().SetTimer(LerpLibrary->TimerHandle, TimerDelegate, 0.03f, true);
    }
}

void ULerpLibrary::InitializeRelativeRotateComponent(UObject* WorldContextObject, USceneComponent* Component, FRotator TargetRotation, float Duration)
{
    ULerpLibrary* LerpLibrary = NewObject<ULerpLibrary>(WorldContextObject);
    LerpLibrary->Component = Component;
    LerpLibrary->TargetRotation = TargetRotation;
    LerpLibrary->Duration = Duration;
    LerpLibrary->ElapsedTime = 0.0f;

    if (Component->GetWorld())
    {
        FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(LerpLibrary, &ULerpLibrary::RotateComponentRelative, 0.01f);
        Component->GetWorld()->GetTimerManager().SetTimer(LerpLibrary->TimerHandle, TimerDelegate, 0.03f, true);
    }

}

void ULerpLibrary::MoveComponent(float DeltaTime)
{
    if (!Component)
    {
        return;
    }

    ElapsedTime += DeltaTime;
    float Alpha = FMath::Clamp(ElapsedTime / Duration, 0.0f, 1.0f);

    FVector NewLocation = FMath::Lerp(StartLocation, TargetLocation, Alpha);
    Component->SetWorldLocation(NewLocation);

    if (Alpha >= 1.0f)
    {
        if (Component->GetWorld())
        {
            Component->GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
        }
    }
}

void ULerpLibrary::RotateComponent(float DeltaTime)
{
    if (!Component)
    {
        return;
    }

    ElapsedTime += DeltaTime;
    float Alpha = FMath::Clamp(ElapsedTime / Duration, 0.0f, 1.0f);

    FRotator NewRotation = FMath::Lerp(StartRotation, TargetRotation, Alpha);
    Component->SetWorldRotation(NewRotation);

    if (Alpha >= 1.0f)
    {
        if (Component->GetWorld())
        {
            Component->GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
        }
    }
}

void ULerpLibrary::RotateComponentRelative(float DeltaTime)
{
    if (!Component)
    {
        return;
    }

    ElapsedTime += DeltaTime;
    float Alpha = FMath::Clamp(ElapsedTime / Duration, 0.0f, 1.0f);

    FRotator CurrentRelativeRotation = Component->GetRelativeRotation();

    FRotator NewRotation = FMath::Lerp(CurrentRelativeRotation, TargetRotation, Alpha);

    Component->SetRelativeRotation(NewRotation);

    if (Alpha >= 1.0f)
    {
        if (Component->GetWorld())
        {
            Component->GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
        }
    }
}

// 原有的函数实现不变

// Implementation of the lerp function
void ULerpLibrary::LerpFloat(float DeltaTime)
{
    if (!ValuePtr || Duration <= 0.0f)
    {
        return;
    }

    ElapsedTime += DeltaTime;
    float Alpha = FMath::Clamp(ElapsedTime / Duration, 0.0f, 1.0f);

    // Perform the linear interpolation
    *ValuePtr = FMath::Lerp(*ValuePtr, TargetValue, Alpha);

    // If lerp is complete, stop the timer
    if (Alpha >= 1.0f)
    {
        if (UWorld* World = GEngine->GetWorldFromContextObjectChecked(this))
        {
            World->GetTimerManager().ClearTimer(LerpFloatTimerHandle);
        }
    }
}



void ULerpLibrary::MoveComponentToSocketLocation(
    USceneComponent* AComponent,
    USceneComponent* BComponent,
    FName SocketName,
    float Duration)
{
    if (!AComponent || !BComponent || Duration <= 0.0f)
    {
        return;
    }

    // 获取初始位置和目标位置
    FVector StartLocation = AComponent->GetComponentLocation();
    FVector TargetLocation = BComponent->GetSocketLocation(SocketName);

    // 初始化动态移动
    InitializeMoveToSocket(AComponent->GetOwner(), AComponent, BComponent, StartLocation, TargetLocation, SocketName, Duration);
}

void ULerpLibrary::InitializeMoveToSocket(
    UObject* WorldContextObject,
    USceneComponent* AComponent,
    USceneComponent* BComponent,
    FVector StartLocation,
    FVector TargetLocation,
    FName SocketName,
    float Duration)
{
    ULerpLibrary* LerpLibrary = NewObject<ULerpLibrary>(WorldContextObject);
    LerpLibrary->AComponent = AComponent;
    LerpLibrary->BComponent = BComponent;
    LerpLibrary->SocketName = SocketName;
    LerpLibrary->StartLocation = StartLocation;
    LerpLibrary->TargetLocation = TargetLocation;
    LerpLibrary->Duration = Duration;
    LerpLibrary->ElapsedTime = 0.0f;

    if (AComponent->GetWorld())
    {
        FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(LerpLibrary, &ULerpLibrary::MoveComponentToSocketLocationUpdate);
        AComponent->GetWorld()->GetTimerManager().SetTimer(LerpLibrary->TimerHandle, TimerDelegate, 0.01f, true);
    }
}

void ULerpLibrary::MoveComponentToSocketLocationUpdate()
{
    if (!AComponent || !BComponent)
    {
        return;
    }

    // 更新目标位置
    TargetLocation = BComponent->GetSocketLocation(SocketName);

    // 计算DeltaTime（固定间隔）
    float DeltaTime = 0.01f;

    // 更新时间与插值因子
    ElapsedTime += DeltaTime;
    float Alpha = FMath::Clamp(ElapsedTime / Duration, 0.0f, 1.0f);

    // 平滑更新位置
    FVector NewLocation = FMath::Lerp(StartLocation, TargetLocation, Alpha);
    AComponent->SetWorldLocation(NewLocation);

    // 判断是否到达目标
    if (Alpha >= 1.0f || FVector::Dist(NewLocation, TargetLocation) <= KINDA_SMALL_NUMBER)
    {
        if (AComponent->GetWorld())
        {
            AComponent->GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
        }
    }
}



void ULerpLibrary::ChangeCameraFOV(UCameraComponent* CameraComponent, float TargetFOV, float Duration)
{
    if (!CameraComponent || Duration <= 0.0f)
    {
        return;
    }

    float CurrentFOV = CameraComponent->FieldOfView;

    ULerpLibrary* LerpInstance = NewObject<ULerpLibrary>(CameraComponent->GetOwner());
    LerpInstance->InitializeCameraFOVChange(CameraComponent->GetOwner(), CameraComponent, CurrentFOV, TargetFOV, Duration);
}



void ULerpLibrary::InitializeCameraFOVChange(UObject* WorldContextObject, UCameraComponent* CameraComponent, float InitialFOV, float TargetFOV, float Duration)
{
    if (!CameraComponent || Duration <= 0.0f)
    {
        return;
    }

    // 创建一个 ULerpLibrary 实例
    ULerpLibrary* LerpInstance = NewObject<ULerpLibrary>(WorldContextObject);
    if (!LerpInstance)
    {
        return;
    }

    // 初始化实例变量
    LerpInstance->Camera = CameraComponent;
    LerpInstance->StartFOV = InitialFOV;
    LerpInstance->EndFOV = TargetFOV;
    LerpInstance->Duration = Duration;
    LerpInstance->ElapsedTime = 0.0f;

    // 启动定时器
    if (CameraComponent->GetWorld())
    {
        FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(LerpInstance, &ULerpLibrary::UpdateCameraFOV, 0.01f);
        CameraComponent->GetWorld()->GetTimerManager().SetTimer(LerpInstance->TimerHandle, TimerDelegate, 0.01f, true);
    }
}



void ULerpLibrary::UpdateCameraFOV(float DeltaTime)
{
    if (!Camera)
    {
        return;
    }

    ElapsedTime += DeltaTime;
    float Alpha = FMath::Clamp(ElapsedTime / Duration, 0.0f, 1.0f);

    float NewFOV = FMath::Lerp(StartFOV, EndFOV, Alpha);
    Camera->SetFieldOfView(NewFOV);

    if (Alpha >= 1.0f)
    {
        if (Camera->GetWorld())
        {
            Camera->GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
        }
    }
}




void ULerpLibrary::MoveComponentToDynamicLocationWithRotation(
    USceneComponent* ParentComponent,
    USceneComponent* ComponentToMove,
    FName SocketName,
    float Duration)
{
    if (!ParentComponent || !ComponentToMove || Duration <= 0.0f)
    {
        return;
    }

    InitializeDynamicMoveComponentWithRotation(
        ParentComponent->GetOwner(),
        ParentComponent,
        ComponentToMove,
        SocketName,
        Duration);
}

void ULerpLibrary::InitializeDynamicMoveComponentWithRotation(
    UObject* WorldContextObject,
    USceneComponent* ParentComponent,
    USceneComponent* ComponentToMove,
    FName SocketName,
    float Duration)
{
    ULerpLibrary* LerpLibrary = NewObject<ULerpLibrary>(WorldContextObject);
    LerpLibrary->Component = ComponentToMove;
    LerpLibrary->ParentComponent = ParentComponent;
    LerpLibrary->SocketName = SocketName;
    LerpLibrary->TargetLocation = ParentComponent->GetSocketLocation(SocketName);
    LerpLibrary->TargetRotation_Quat = ParentComponent->GetSocketRotation(SocketName).Quaternion();
    LerpLibrary->Duration = Duration;
    LerpLibrary->ElapsedTime = 0.0f;

    if (ParentComponent->GetWorld())
    {
        FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(LerpLibrary, &ULerpLibrary::MoveComponentToDynamicLocationWithRotationUpdate);
        ParentComponent->GetWorld()->GetTimerManager().SetTimer(LerpLibrary->TimerHandle, TimerDelegate, 0.01f, true);
    }
}

void ULerpLibrary::MoveComponentToDynamicLocationWithRotationUpdate()
{
    if (!Component || !ParentComponent)
    {
        return;
    }

    // 第0步: 获取当前的RelativeLocation
    FVector CurrentRelativeLocation =Component->GetRelativeLocation();


    // 获取 Socket 和父组件的世界旋转
    FRotator CurrentRelativeRotation = Component->GetRelativeRotation();


    // 更新时间与插值因子
    float DeltaTime = 0.01f;
    ElapsedTime += DeltaTime;
    float Alpha = FMath::Clamp(ElapsedTime / Duration, 0.0f, 1.0f);


    // 第三步：根据 Alpha 来设置 NewRelativeSocketLocation 和旋转
    // 计算新的相对位置插值
    FVector NewLocation = FMath::Lerp(CurrentRelativeLocation, FVector::ZeroVector, Alpha);
    Component->SetRelativeLocation(NewLocation); // 设置新的相对位置

    // 计算新的旋转插值，使用四元数插值
    FRotator NewRotation = FMath::Lerp(CurrentRelativeRotation, FRotator::ZeroRotator, Alpha); // 使用 Slerp 插值旋转

    Component->SetRelativeRotation(NewRotation); // 设置新的旋转


    // 判断是否到达目标
    if (Alpha >= 1.0f ||
        (FVector::Dist(NewLocation, TargetLocation) <= KINDA_SMALL_NUMBER &&  // 目标位置判断
            FQuat(NewRotation).AngularDistance(FQuat(TargetRotation)) <= KINDA_SMALL_NUMBER))  // 目标旋转判断
    {
        // 停止定时器
        if (Component->GetWorld())
        {
            Component->GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
        }
    }
}


void ULerpLibrary::MoveComponentToZeroWithLerp(
    USceneComponent* ParentComponent,
    USceneComponent* ComponentToMove,
    float Duration)
{
    if (!ParentComponent || !ComponentToMove || Duration <= 0.0f)
    {
        return;
    }

    // 初始化A组件（ComponentToMove）的位置和旋转（零位置和零旋转）
    FVector StartLocation = ComponentToMove->GetComponentLocation();
    FVector TargetLocation = FVector::ZeroVector;
    FQuat StartRotation = ComponentToMove->GetComponentQuat();
    FQuat TargetRotation = FQuat::Identity;

    // 初始化移动逻辑
    InitializeDynamicMoveComponentWithRotation(
        ParentComponent->GetOwner(),
        ParentComponent,
        ComponentToMove,
        Duration);
}

void ULerpLibrary::InitializeDynamicMoveComponentWithRotation(
    UObject* WorldContextObject,
    USceneComponent* ParentComponent,
    USceneComponent* ComponentToMove,
    float Duration)
{
    ULerpLibrary* LerpLibrary = NewObject<ULerpLibrary>(WorldContextObject);
    LerpLibrary->Component = ComponentToMove;
    LerpLibrary->ParentComponent = ParentComponent;
    LerpLibrary->Duration = Duration;
    LerpLibrary->ElapsedTime = 0.0f;

    if (ParentComponent->GetWorld())
    {
        FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(LerpLibrary, &ULerpLibrary::MoveComponentToZeroWithLerpUpdate);
        ParentComponent->GetWorld()->GetTimerManager().SetTimer(LerpLibrary->TimerHandle, TimerDelegate, 0.01f, true);
    }
}

void ULerpLibrary::MoveComponentToZeroWithLerpUpdate()
{
    if (!Component || !ParentComponent)
    {
        return;
    }

    // 计算目标位置与旋转（零位置和零旋转）
    TargetLocation = FVector::ZeroVector;
    TargetRotation_Quat = FQuat::Identity;

    // 计算DeltaTime（可以通过Timer间隔固定）
    float DeltaTime = 0.01f;

    // 更新时间与插值因子
    ElapsedTime += DeltaTime;
    float Alpha = FMath::Clamp(ElapsedTime / Duration, 0.0f, 1.0f);

    // 平滑更新位置
    FVector NewLocation = FMath::Lerp(StartLocation, TargetLocation, Alpha);
    Component->SetRelativeLocation(NewLocation);

    // 平滑更新旋转
    FQuat NewRotation = FQuat::Slerp(StartRotation_Quat, TargetRotation_Quat, Alpha);
    Component->SetRelativeRotation(NewRotation);

    // 判断是否到达目标
    if (Alpha >= 1.0f || (FVector::Dist(NewLocation, TargetLocation) <= KINDA_SMALL_NUMBER &&
        FQuat::ErrorAutoNormalize(TargetRotation_Quat, NewRotation) <= KINDA_SMALL_NUMBER))
    {
        if (Component->GetWorld())
        {
            Component->GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
        }
    }
}



void ULerpLibrary::MoveComponentRelativeToParent(USceneComponent* ParentComponent, USceneComponent* ComponentToMove, FVector TargetRelativeLocation, float Duration)
{
    if (!ParentComponent || !ComponentToMove || Duration <= 0.0f)
    {
        return;
    }


    // 初始化平滑插值
    InitializeMoveComponentRelativeToParent(
        ParentComponent->GetOwner(),
        ParentComponent,
        ComponentToMove,
        TargetRelativeLocation,
        Duration);
}


void ULerpLibrary::InitializeMoveComponentRelativeToParent(
    UObject* WorldContextObject,
    USceneComponent* ParentComponent,
    USceneComponent* ComponentToMove,
    FVector TargetRelativeLocation,
    float Duration)
{
    ULerpLibrary* LerpLibrary = NewObject<ULerpLibrary>(WorldContextObject);
    LerpLibrary->Component = ComponentToMove;
    LerpLibrary->ParentComponent = ParentComponent;
    LerpLibrary->TargetRelativeLocation = TargetRelativeLocation;
    LerpLibrary->Duration = Duration;
    LerpLibrary->ElapsedTime = 0.0f;

    if (ParentComponent->GetWorld())
    {
        FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(LerpLibrary, &ULerpLibrary::MoveComponentRelativeToParentUpdate);
        ParentComponent->GetWorld()->GetTimerManager().SetTimer(LerpLibrary->TimerHandle, TimerDelegate, 0.01f, true);
    }
}


void ULerpLibrary::MoveComponentRelativeToParentUpdate()
{
    if (!Component || !ParentComponent)
    {
        return;
    }

    // 获取组件的相对位置，不需要使用世界坐标
    FVector CurrentRelativeLocation = Component->GetRelativeLocation();


    // 计算目标位置（目标位置相对父组件）
    FVector BRelativeLocation = this->TargetRelativeLocation;

    // 计算DeltaTime（固定间隔）
    float DeltaTime = 0.01f;

    // 更新时间与插值因子
    ElapsedTime += DeltaTime;
    float Alpha = FMath::Clamp(ElapsedTime / Duration, 0.0f, 1.0f);

    // 计算新的相对位置
    FVector NewRelativeLocation = FMath::Lerp(CurrentRelativeLocation, BRelativeLocation, Alpha);

    // 设置相对位置
    Component->SetRelativeLocation(NewRelativeLocation);

    // 判断是否到达目标
    if (Alpha >= 1.0f || FVector::Dist(NewRelativeLocation, BRelativeLocation) <= KINDA_SMALL_NUMBER)
    {
        if (Component->GetWorld())
        {
            Component->GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
        }
    }
}
