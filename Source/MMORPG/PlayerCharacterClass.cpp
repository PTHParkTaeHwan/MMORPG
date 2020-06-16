// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacterClass.h"
#include "CoreTypes.h"
#include "Components/WidgetComponent.h"
#include "Components/TextRenderComponent.h"
#include "Animation/SkeletalMeshActor.h"


// Sets default values
APlayerCharacterClass::APlayerCharacterClass()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	//스켈레탈메시 설정
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CHARATERMESH(TEXT("SkeletalMesh'/Game/ParagonShinbi/Characters/Heroes/Shinbi/Meshes/Shinbi.Shinbi'"));
	if (CHARATERMESH.Succeeded())                                         
	{
		GetMesh()->SetSkeletalMesh(CHARATERMESH.Object);
	}
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	static ConstructorHelpers::FClassFinder<UAnimInstance> PlayerAnim(TEXT("AnimBlueprint'/Game/ParagonShinbi/Characters/Heroes/Shinbi/Shinbi_AnimBlueprint.Shinbi_AnimBlueprint_C'"));
	if (PlayerAnim.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(PlayerAnim.Class);
	}

	//카메라 설정
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);
	SpringArm->TargetArmLength = 410.0f;
	SpringArm->SetRelativeRotation(FRotator::ZeroRotator);
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bInheritPitch = true;
	SpringArm->bInheritRoll = true;
	SpringArm->bInheritYaw = true;
	SpringArm->bDoCollisionTest = true;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);

	//TestEffect
	TestEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("SKILL_1"));
	TestEffect->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_SKILL_1(TEXT("ParticleSystem'/Game/InfinityBladeEffects/Effects/FX_Ambient/Fire/P_Fire_largeArea_Trans_mobile.P_Fire_largeArea_Trans_mobile'"));
	if (P_SKILL_1.Succeeded())
	{
		TestEffect->SetTemplate(P_SKILL_1.Object);
		TestEffect->bAutoActivate = false;
	}
	TestEffect->SetIsReplicated(true);

	ChatText = CreateDefaultSubobject<UTextRenderComponent>("ChatText");
	ChatText->SetRelativeLocation(FVector(0, 0, 100));
	ChatText->SetHorizontalAlignment(EHTA_Center);
	ChatText->SetupAttachment(RootComponent);


}

// Called when the game starts or when spawned
void APlayerCharacterClass::BeginPlay()
{
	Super::BeginPlay();
	CurrentMessage = "";

}

// Called every frame
void APlayerCharacterClass::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerCharacterClass::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &APlayerCharacterClass::UpDown);
	PlayerInputComponent->BindAxis(TEXT("LeftRight"), this, &APlayerCharacterClass::LeftRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APlayerCharacterClass::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &APlayerCharacterClass::Turn);
	PlayerInputComponent->BindAction(TEXT("Fire"), EInputEvent::IE_Pressed, this, &APlayerCharacterClass::Bomb);

}

//캐릭터 움직임
void APlayerCharacterClass::UpDown(float NewAxisValue)
{
	AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X), NewAxisValue);
}
void APlayerCharacterClass::LeftRight(float NewAxisValue)
{
	AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::Y), NewAxisValue);
}
//카메라 움직임
void APlayerCharacterClass::LookUp(float NewAxisValue)
{
	AddControllerPitchInput(NewAxisValue);
}
void APlayerCharacterClass::Turn(float NewAxisValue)
{
	AddControllerYawInput(NewAxisValue);
}
//스폰
void APlayerCharacterClass::Bomb()
{
	ServerSpawn();

	if (HasAuthority())
	{
		ServerSpawn();
	}
}

void APlayerCharacterClass::ServerSpawn_Implementation()
{
	if (HasAuthority())
	{
		//ServerSpawn();
		ServerSpawnNetMulticast();
	}
}

bool APlayerCharacterClass::ServerSpawn_Validate()
{
	return true;
}

void APlayerCharacterClass::ServerSpawnNetMulticast_Implementation()
{
	TestEffect->Activate(true);
}

bool APlayerCharacterClass::ServerSpawnNetMulticast_Validate()
{
	return true;
}

void APlayerCharacterClass::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APlayerCharacterClass, CurrentMessage);
}


void APlayerCharacterClass::SendChatMessage(const FString& Message)
{
	CurrentMessage = Message;
	UpdateChatText();

	FTimerHandle DummyHandle;
	GetWorldTimerManager().SetTimer(DummyHandle, this, &APlayerCharacterClass::ChearChatMessage, 5.f);
}

void APlayerCharacterClass::AttemptToSendChatMessage(const FString& Message)
{
	if (!HasAuthority())
	{
		SeverSendChatMessage(Message);
	}
	else SendChatMessage(Message);
}


void APlayerCharacterClass::SeverSendChatMessage_Implementation(const FString& Message)
{
	SendChatMessage(Message);
}
bool APlayerCharacterClass::SeverSendChatMessage_Validate(const FString& Message)
{
	if (Message.Len() < 255)
	{
		return true;
	}
	else return false;
}

void APlayerCharacterClass::OnRep_CurrentMessage()
{
	UpdateChatText();
}

void APlayerCharacterClass::ChearChatMessage()
{
	CurrentMessage = "";
	UpdateChatText();
}

void APlayerCharacterClass::UpdateChatText()
{
	ChatText->SetText(FText::FromString(CurrentMessage));
}
