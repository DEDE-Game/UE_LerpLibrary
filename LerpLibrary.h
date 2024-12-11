#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LerpLibrary.generated.h"

UCLASS()
class LUXUN2024_API ULerpLibrary : public UObject
{
    GENERATED_BODY()

public:
    // ��ֵ�ƶ�λ��
    UFUNCTION(BlueprintCallable, Category = "Lerp")
    static void MoveActorToLocation(AActor* Actor, FVector TargetLocation, float Duration);

    UFUNCTION(BlueprintCallable, Category = "Lerp")
    static void MoveComponentToLocation(USceneComponent* Component, FVector TargetLocation, float Duration);

   

    // ��ֵ��ת
    UFUNCTION(BlueprintCallable, Category = "Lerp")
    static void RotateActorToRotation(AActor* Actor, FRotator TargetRotation, float Duration);

    UFUNCTION(BlueprintCallable, Category = "Lerp")
    static void RotateComponentToRotation(USceneComponent* Component, FRotator TargetRotation, float Duration);

    UFUNCTION(BlueprintCallable, Category = "Lerp")
    static void RotateComponentToRelativeRotation(USceneComponent* Component, FRotator TargetRotation, float Duration);


    // ��ֵ����
    UFUNCTION(BlueprintCallable, Category = "Lerp")
    static void ScaleActorToScale(AActor* Actor, FVector TargetScale, float Duration);


    //floatֵ������lerp
    UFUNCTION(BlueprintCallable, Category = "Lerp")
    static  void LerpFloatToTarget(UObject* WorldContextObject, float& CurrentValue, float TargetValue, float Duration);


    //MoveComponentToRelativeLocation
    UFUNCTION(BlueprintCallable, Category = "Lerp")
    static void MoveComponentToDynamicLocation(USceneComponent* ParentComponent, USceneComponent* ComponentToMove, FName SocketName, float Duration);

    static void InitializeDynamicMoveComponent(UObject* WorldContextObject, USceneComponent* ParentComponent, USceneComponent* ComponentToMove, FVector StartLocation, FVector TargetLocation, FName SocketName, float Duration);

    void MoveComponentToDynamicLocationUpdate();



    //��AComponent��λ�Ƶ�BComponent��Socketλ�ã���̬����
    UFUNCTION(BlueprintCallable, Category = "Lerp")
    static void MoveComponentToSocketLocation(USceneComponent* AComponent, USceneComponent* BComponent, FName SocketName, float Duration);    

    static void InitializeMoveToSocket(UObject* WorldContextObject, USceneComponent* AComponent, USceneComponent* BComponent, FVector StartLocation, FVector TargetLocation, FName SocketName, float Duration);

    void MoveComponentToSocketLocationUpdate();



    //����CameraFOV
    static void ChangeCameraFOV(class UCameraComponent* CameraComponent, float TargetFOV, float Duration);

    static void InitializeCameraFOVChange(UObject* WorldContextObject, class UCameraComponent* CameraComponent, float InitialFOV, float TargetFOV, float Duration);

    void UpdateCameraFOV(float DeltaTime);


    //����Rotation��movecomponent��Ŀ��λ��
    static void MoveComponentToDynamicLocationWithRotation(USceneComponent* ParentComponent, USceneComponent* ComponentToMove, FName SocketName, float Duration);

    static  void InitializeDynamicMoveComponentWithRotation(UObject* WorldContextObject, USceneComponent* ParentComponent, USceneComponent* ComponentToMove, FName SocketName, float Duration);

    void MoveComponentToDynamicLocationWithRotationUpdate();

    //��Component�ڸ������lerp����
    UFUNCTION(BlueprintCallable)
    static void MoveComponentToZeroWithLerp(USceneComponent* ParentComponent, USceneComponent* ComponentToMove, float Duration);

    static void InitializeDynamicMoveComponentWithRotation(UObject* WorldContextObject, USceneComponent* ParentComponent, USceneComponent* ComponentToMove,float Duration);

    void MoveComponentToZeroWithLerpUpdate();


    //�Ӽ�����ڸ�����location��lerp����
    static void MoveComponentRelativeToParent(USceneComponent* ParentComponent, USceneComponent* ComponentToMove, FVector TargetRelativeLocation, float Duration);

    static void InitializeMoveComponentRelativeToParent(
        UObject* WorldContextObject,
        USceneComponent* ParentComponent,
        USceneComponent* ComponentToMove,
        FVector TargetrelativeLocation,
        float Duration);

    void MoveComponentRelativeToParentUpdate();



    //�ı������FOV
   // void ChangeCameraFOV(UCameraComponent* CameraComponent, float TargetFOV, float Duration);

  //  void UpdateCameraFOV(float DeltaTime);



private:
    // �������������ڳ�ʼ���ƶ�
    static void InitializeMove(UObject* WorldContextObject, AActor* Actor, FVector StartLocation, FVector TargetLocation, float Duration);

    static void InitializeMoveComponent(UObject* WorldContextObject, USceneComponent* Component, FVector StartLocation, FVector TargetLocation, float Duration);

    // �������������ڳ�ʼ����ת
    static void InitializeRotate(UObject* WorldContextObject, AActor* Actor, FRotator StartRotation, FRotator TargetRotation, float Duration);

    static void InitializeRotateComponent(UObject* WorldContextObject, USceneComponent* Component, FRotator StartRotation, FRotator TargetRotation, float Duration);

    static void InitializeRelativeRotateComponent(UObject* WorldContextObject, USceneComponent* Component, FRotator TargetRotation, float Duration);

    // �������������ڳ�ʼ������
    static void InitializeScale(UObject* WorldContextObject, AActor* Actor, FVector StartScale, FVector TargetScale, float Duration);

    // ������ƶ��߼�
    void MoveActor(float DeltaTime);
    void MoveComponent(float DeltaTime);



    // �������ת�߼�
    void RotateActor(float DeltaTime);
    void RotateComponent(float DeltaTime);

    void RotateComponentRelative(float DeltaTime);

    // ����������߼�
    void ScaleActor(float DeltaTime);

    // ��������Բ�ֵfloat
    void LerpFloat(float DeltaTime);


    



    FVector StartRelativeLocation;
    FVector TargetRelativeLocation;


    AActor* Actor;
    USceneComponent* Component;
    USceneComponent* AComponent;
    USceneComponent* BComponent;
    USceneComponent* ParentComponent;
    FName SocketName;
    FVector StartLocation;
    FVector TargetLocation;
    FRotator StartRotation;
    FRotator TargetRotation;
    FQuat StartRotation_Quat;
    FQuat TargetRotation_Quat;
    FVector StartScale;
    FVector TargetScale;
    float Duration;
    float ElapsedTime;
    FTimerHandle TimerHandle;

    class UCameraComponent* Camera;
    float StartFOV;
    float EndFOV;


    // Timer handle for the lerping process
    FTimerHandle LerpFloatTimerHandle;

    // Variables for lerping
    float* ValuePtr;
    float TargetValue;


};
