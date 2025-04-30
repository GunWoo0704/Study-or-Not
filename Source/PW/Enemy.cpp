#include "Enemy.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Projectile.h"
#include "Kismet/GameplayStatics.h"
#include "EnemySpawner.h"
#include "Components/WidgetComponent.h"
#include "UMySpeechBubbleWidget.h" 
#include "PWGameMode.h"
#include "Sound/SoundBase.h"

// Sets default values
AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	// 래그돌 조건 초기화
	bCanRagdoll = false;
	bPreviousCanRagdoll = false;

	// 기본 타이머 시간 설정
	MinTimeBetweenAnimations = 3.0f;
	MaxTimeBetweenAnimations = 30.0f;

	// 충돌 이벤트 등록
	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &AEnemy::OnHit);

	// 래그돌 후 제거 지연 시간 초기화
	DestroyAfterRagdollDelay = 4.0f;

	// 사운드 및 점수 페널티 초기화
	RagdollSound = nullptr;
	WrongHitSound = nullptr;
	WrongHitScorePenalty = 5;

	SpeechBubbleWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("SpeechBubble"));
	SpeechBubbleWidget->SetupAttachment(GetMesh());
	SpeechBubbleWidget->SetWidgetSpace(EWidgetSpace::Screen);
	SpeechBubbleWidget->SetDrawSize({ 200,100 });
	SpeechBubbleWidget->SetVisibility(false);
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	// 명시적으로 false로 초기화
	bCanRagdoll = false;
	bPreviousCanRagdoll = false;

	// 첫 랜덤 타이머 설정
	SetNewRandomTimer();
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// bCanRagdoll 값이 false에서 true로 변경되었을 때만 애니메이션 재생
	if (bCanRagdoll && !bPreviousCanRagdoll)
	{
		PlaySpecialAnimation();
	}

	// 현재 상태를 이전 상태로 저장
	bPreviousCanRagdoll = bCanRagdoll;
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AEnemy::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// 충돌한 액터가 투사체인지 확인
	AProjectile* Projectile = Cast<AProjectile>(OtherActor);
	if (Projectile)
	{
		// 데미지 처리
		float DamageAmount = 10.0f;
		TSubclassOf<UDamageType> DamageType = UDamageType::StaticClass();
		UGameplayStatics::ApplyDamage(this, DamageAmount, nullptr, Projectile->GetOwner(), DamageType);
	}
}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	// 월드 객체 가져오기
	UWorld* World = GetWorld();
	if (!World) return ActualDamage;

	// APWGameMode로 캐스트
	APWGameMode* GameMode = Cast<APWGameMode>(World->GetAuthGameMode());
	if (!GameMode) return ActualDamage;

	// 래그돌 상태 확인 후 처리
	if (bCanRagdoll)
	{
		// 래그돌 상태(특수 애니메이션 재생 중)에서는 정상적인 래그돌 처리
		// EnableRagdoll() 안에서 이미 점수 추가 및 RagdollSound 재생을 처리함
		EnableRagdoll();
	}
	else
	{
		// 래그돌 상태가 아닐 때 맞추면 점수 차감
		GameMode->AddScore(-WrongHitScorePenalty);

		// 잘못된 타이밍에 맞췄을 때의 사운드 재생
		if (WrongHitSound)
		{
			UGameplayStatics::PlaySoundAtLocation(
				this,
				WrongHitSound,
				GetActorLocation(),
				1.0f,
				1.0f
			);
		}
	}

	return ActualDamage;
}

void AEnemy::EnableRagdoll()
{
	GetCapsuleComponent()->OnComponentHit.RemoveDynamic(this, &AEnemy::OnHit);

	if (UWorld* World = GetWorld())
	{
		// APWGameMode로 캐스트해서 AddScore 호출
		if (APWGameMode* GameMode = Cast<APWGameMode>(World->GetAuthGameMode()))
		{
			GameMode->AddScore(20);
		}
	}

	// 래그돌 사운드 재생
	if (RagdollSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,           // 월드 컨텍스트 객체
			RagdollSound,   // 재생할 사운드
			GetActorLocation(), // 사운드 재생 위치
			0.7f,           // 볼륨 배수
			1.0f            // 피치 배수
		);
	}

	// 래그돌 물리 활성화
	USkeletalMeshComponent* SkelMesh = GetMesh();
	if (SkelMesh)
	{
		// 캡슐 콜라이더 비활성화
		UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
		if (CapsuleComp)
		{
			CapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}

		// 캐릭터 무브먼트 비활성화
		GetCharacterMovement()->SetMovementMode(MOVE_None);

		// 래그돌 활성화
		SkelMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
		SkelMesh->SetCollisionResponseToAllChannels(ECR_Block);
		SkelMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
		SkelMesh->SetSimulatePhysics(true);

		// (C) 랜덤 대사 보여주기
		ShowRandomDialogue();

		// (D) 위젯 보이기
		SpeechBubbleWidget->SetVisibility(true);

		// 액터 제거 타이머 설정
		GetWorldTimerManager().SetTimer(
			DestroyTimerHandle,
			FTimerDelegate::CreateUObject(this, &AEnemy::DestroyActor),
			DestroyAfterRagdollDelay,
			false
		);
	}
}

void AEnemy::PlaySpecialAnimation()
{
	// 랜덤한 몽타주 선택
	int32 Index = FMath::RandRange(0, SpecialAnimMontages.Num() - 1);
	UAnimMontage* SelectedMontage = SpecialAnimMontages[Index];

	USkeletalMeshComponent* MeshComp = GetMesh();
	if (MeshComp)
	{
		UAnimInstance* AnimInstance = MeshComp->GetAnimInstance();
		if (AnimInstance)
		{
			if (AnimInstance->Montage_IsPlaying(SelectedMontage)) return;

			float Duration = AnimInstance->Montage_Play(SelectedMontage);

			// 콜백 연결
			AnimInstance->OnMontageEnded.RemoveDynamic(this, &AEnemy::OnAnimationMontageEnded);
			AnimInstance->OnMontageEnded.AddDynamic(this, &AEnemy::OnAnimationMontageEnded);

			// 현재 몽타주 추적 (필요시)
			LastPlayedMontage = SelectedMontage;
		}
	}
}

void AEnemy::OnAnimationMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage == LastPlayedMontage)
	{
		bCanRagdoll = false;
		SetNewRandomTimer();
		LastPlayedMontage = nullptr;
	}
}

void AEnemy::TriggerRagdollAnimation()
{
	bCanRagdoll = true;
}

void AEnemy::SetNewRandomTimer()
{
	float RandomTime = FMath::RandRange(MinTimeBetweenAnimations, MaxTimeBetweenAnimations);
	GetWorldTimerManager().SetTimer(
		RagdollTimerHandle,
		this,
		&AEnemy::TriggerRagdollAnimation,
		RandomTime,
		false
	);
}

void AEnemy::DestroyActor()
{
	if (OwningSpawner)
	{
		OwningSpawner->OnEnemyDestroyed(GetActorLocation(), GetActorRotation());
	}

	Destroy();
}

void AEnemy::InitializeRandomAnimation()
{
	bCanRagdoll = false;
	bPreviousCanRagdoll = false;

	SetNewRandomTimer();
}

void AEnemy::ShowRandomDialogue()
{
	if (DialogueLines.Num() == 0) return;

	int32 Idx = FMath::RandRange(0, DialogueLines.Num() - 1);
	FText Chosen = DialogueLines[Idx];

	if (auto* UW = Cast<UMySpeechBubbleWidget>(SpeechBubbleWidget->GetUserWidgetObject()))
	{
		UW->SetDialogueText(Chosen);
	}
}