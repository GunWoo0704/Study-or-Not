// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

class AEnemySpawner;  // 전방 선언

UCLASS(Blueprintable)

class PW_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// 피격 함수
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	// 래그돌 활성화 함수
	UFUNCTION(BlueprintCallable, Category = "Ragdoll")
	void EnableRagdoll();

	// 데미지 처리 함수
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	// 래그돌 활성화 조건
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ragdoll")
	bool bCanRagdoll;

	// 애니메이션 몽타주
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	TArray<UAnimMontage*> SpecialAnimMontages;

	// 애니메이션 재생 함수
	UFUNCTION(BlueprintCallable, Category = "Animation")
	void PlaySpecialAnimation();

	// 액터 제거 타이머
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ragdoll")
	float DestroyAfterRagdollDelay;

	// 랜덤 애니메이션 초기화 함수
	UFUNCTION(BlueprintCallable, Category = "Animation")
	void InitializeRandomAnimation();

protected:
	UAnimMontage* LastPlayedMontage = nullptr;

	// 몽타주 종료 콜백 함수
	UFUNCTION()
	void OnAnimationMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	// 이전 프레임의 Ragdoll 상태를 저장
	bool bPreviousCanRagdoll;

	// 타이머 핸들
	FTimerHandle RagdollTimerHandle;
	FTimerHandle DestroyTimerHandle;

	// 랜덤 타이머 설정을 위한 최소, 최대 시간
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	float MinTimeBetweenAnimations;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	float MaxTimeBetweenAnimations;

	// 타이머 콜백 함수
	UFUNCTION()
	void TriggerRagdollAnimation();

	// 새로운 랜덤 타이머 설정 함수
	void SetNewRandomTimer();

	// 액터 제거 함수
	UFUNCTION()
	void DestroyActor();
	// Enemy.h에 추가

public:
	// 스포너 참조
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	class AEnemySpawner* OwningSpawner;

};