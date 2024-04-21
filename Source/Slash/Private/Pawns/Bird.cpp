#include "Pawns/Bird.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"

ABird::ABird() {
	PrimaryActorTick.bCanEverTick = true;

	Capsule = CreateDefaultSubobject<UCapsuleComponent>("Capsule");
	Capsule->SetCapsuleHalfHeight(20.f);
	Capsule->SetCapsuleRadius(15.f);
	SetRootComponent(Capsule);

	BirdMesh = CreateDefaultSubobject<USkeletalMeshComponent>("BirdMesh");
	BirdMesh->SetupAttachment(Capsule);

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 300.f;

	ViewCamera = CreateDefaultSubobject<UCameraComponent>("ViewCamera");
	ViewCamera->SetupAttachment(CameraBoom);

	AutoPossessPlayer = EAutoReceiveInput::Player0;
	
}

void ABird::BeginPlay() {
	Super::BeginPlay();
}

void ABird::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void ABird::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ABird::MoveForward);
	PlayerInputComponent->BindAxis("Turn", this, &ABird::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &ABird::LookUp);
}

void ABird::MoveForward(float Value) {
	if((Controller) && (Value != 0.f)){
		FVector Forward = GetActorForwardVector();
		AddMovementInput(Forward, Value);
	}
}

void ABird::Turn(float Value) {
	AddControllerYawInput(Value);
}
void ABird::LookUp(float Value) {
	AddControllerPitchInput(Value);
}
