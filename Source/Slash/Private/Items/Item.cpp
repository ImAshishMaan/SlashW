#include "Items/Item.h"
#include "Slash/DebugMacros.h"

AItem::AItem() {
	PrimaryActorTick.bCanEverTick = true;
}

void AItem::BeginPlay() {
	Super::BeginPlay();

	UWorld* World = GetWorld();
	
	
	SetActorLocation(FVector(0.f, 0.f, 50.f));
	SetActorRotation(FRotator(0.f, 0.f, 90.f));

	FVector Location = GetActorLocation();
	FVector Forward = GetActorForwardVector();

	DRAW_SPHERE(Location);
	DRAW_VECTOR(Location, Location + Forward * 100.f);
}

void AItem::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}
