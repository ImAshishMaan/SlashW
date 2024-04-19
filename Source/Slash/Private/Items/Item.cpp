#include "Items/Item.h"
#include "Slash/DebugMacros.h"

AItem::AItem() {
	PrimaryActorTick.bCanEverTick = true;
}

void AItem::BeginPlay() {
	Super::BeginPlay();
}

void AItem::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}
