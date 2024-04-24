#include "Characters/SlashCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GroomComponent.h"
#include "Items/Weapons/Weapon.h"

ASlashCharacter::ASlashCharacter() {
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true; // move character direction to character movement
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f); // character move rotation speed

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 300.f;

	ViewCamera = CreateDefaultSubobject<UCameraComponent>("ViewCamera");
	ViewCamera->SetupAttachment(CameraBoom);

	Hair = CreateDefaultSubobject<UGroomComponent>("Hair");
	Hair->SetupAttachment(GetMesh());
	Hair->AttachmentName = FString("head");

	Eyebrows = CreateDefaultSubobject<UGroomComponent>("Eyebrows");
	Eyebrows->SetupAttachment(GetMesh());
	Eyebrows->AttachmentName = FString("head");
}

void ASlashCharacter::BeginPlay() {
	Super::BeginPlay();
}

void ASlashCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void ASlashCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASlashCharacter::MoveForward);
	PlayerInputComponent->BindAxis("LookUp", this, &ASlashCharacter::LookUp);
	PlayerInputComponent->BindAxis("Turn", this, &ASlashCharacter::Turn);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASlashCharacter::MoveRight);
	
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Equip", IE_Pressed, this, &ASlashCharacter::EKeyPressed);
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &ASlashCharacter::Attack);
	
}

void ASlashCharacter::MoveForward(float Value) {
	if(ActionState != EActionState::EAS_Unoccupied) return;
	if((Controller) && (Value != 0.f)){
		// find out which way is forward
		const FRotator ControlRotation = GetControlRotation();
		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
		
	}
}

void ASlashCharacter::Turn(float Value) {
	AddControllerYawInput(Value);
}

void ASlashCharacter::LookUp(float Value) {
	AddControllerPitchInput(Value);
}

void ASlashCharacter::MoveRight(float Value) {
	if(ActionState != EActionState::EAS_Unoccupied) return;
	if((Controller) && (Value != 0.f)){
		// find out which way is right
		const FRotator ControlRotation = GetControlRotation();
		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}

void ASlashCharacter::EKeyPressed() {
	AWeapon* OverlappingWeapon = Cast<AWeapon>(OverlappingItem);
	if(OverlappingWeapon) {
		OverlappingWeapon->Equip(GetMesh(), "RightHandSocket");
		CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
		OverlappingItem = nullptr;
		EquippedWeapon = OverlappingWeapon;
	}else {
		if(CanDisarm()) {
			PlayEquipMontage(FName("Unequip"));
			CharacterState = ECharacterState::ECS_UnEquipped;
			ActionState = EActionState::EAS_EquippingWeapon;
		} else if(CanArm()) {
			PlayEquipMontage(FName("Equip"));
			CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
			ActionState = EActionState::EAS_EquippingWeapon;
		}
	}
}

void ASlashCharacter::Attack() {
	if(CanAttack()) {
		PlayAttackMontage();
		ActionState = EActionState::EAS_Attacking;
	}
}

bool ASlashCharacter::CanAttack() {
	return ActionState == EActionState::EAS_Unoccupied &&
		CharacterState != ECharacterState::ECS_UnEquipped;
}


bool ASlashCharacter::CanDisarm() {
	return CharacterState != ECharacterState::ECS_UnEquipped &&
		ActionState == EActionState::EAS_Unoccupied;
}

bool ASlashCharacter::CanArm() {
	return ActionState == EActionState::EAS_Unoccupied &&
		CharacterState == ECharacterState::ECS_UnEquipped &&
		EquippedWeapon;
}

void ASlashCharacter::Disarm() {
	if(EquippedWeapon) {
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("SpineSocket"));
	}
}

void ASlashCharacter::Arm() {
	if(EquippedWeapon) {
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("RightHandSocket"));
	}
}

void ASlashCharacter::FinishEquipping() {
	ActionState = EActionState::EAS_Unoccupied;
}

void ASlashCharacter::PlayAttackMontage() {
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(AnimInstance && AttackMontage) {
		AnimInstance->Montage_Play(AttackMontage);
		const int32 Selection = FMath::RandRange(0, 1);
		FName SectionName = FName();
		switch(Selection) {
		case 0:
			SectionName = FName("Attack1");
			break;
		case 1:
			SectionName = FName("Attack2");
			break;
		default:
			break;
		}
		AnimInstance->Montage_JumpToSection(SectionName, AttackMontage);
	}
}

void ASlashCharacter::PlayEquipMontage(FName SectionName) {
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(AnimInstance && EquipMontage) {
		AnimInstance->Montage_Play(EquipMontage);
		AnimInstance->Montage_JumpToSection(SectionName, EquipMontage);
	}
}

void ASlashCharacter::AttackEnd() {
	ActionState = EActionState::EAS_Unoccupied;
}


