// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "EngineUtils.h"
#include "HealthComponent.h"
#include "PlayerCharacter.h"
#include "AGP/Pathfinding/PathfindingSubsystem.h"
#include "Perception/PawnSensingComponent.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>("Pawn Sensing Component");
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	PathfindingSubsystem = GetWorld()->GetSubsystem<UPathfindingSubsystem>();
	if (PathfindingSubsystem)
	{
		CurrentPath = PathfindingSubsystem->GetRandomPath(GetActorLocation());
	} else
	{
		UE_LOG(LogTemp, Error, TEXT("Unable to find the PathfindingSubsystem"))
	}
	if (PawnSensingComponent)
	{
		PawnSensingComponent->OnSeePawn.AddDynamic(this, &AEnemyCharacter::OnSensedPawn);
	}
}

void AEnemyCharacter::MoveAlongPath()
{
	// Execute the path. Should be called each tick.

	// If the path is empty do nothing.
	//if (CurrentPath.IsEmpty()) return;
		
	// If AI is in Engage state, and have direct sight on player, 
	// Move directly towards player. 

	//if (SensedCharacter && CurrentState == EEnemyState::Engage) 
	//if (LastKnownPlayerLocation != FVector(0, 0, 0) && CurrentState != EEnemyState::Evade)
	if (LastKnownPlayerLocation != FVector(0, 0, 0) && CurrentState != EEnemyState::Evade && SensedCharacter)
	//if (SensedCharacter && CurrentState == EEnemyState::Engage)
	{
		UE_LOG(LogTemp, Display, TEXT("AI knows LastKnownPlayerLocation, moving towards there"));
		UE_LOG(LogTemp, Display, TEXT("Distance to player is: %f"), FVector::Distance(GetActorLocation(), LastKnownPlayerLocation));

		//if (FVector::Distance(GetActorLocation(), LastKnownPlayerLocation) > PathfindingError)
		if (FVector::Distance(GetActorLocation(), LastKnownPlayerLocation) > 300)
		{
			//LastKnownPlayerLocation = FVector(0, 0, 0);
			// Move directly towards player
			FVector MovementDirection = LastKnownPlayerLocation - GetActorLocation();
			MovementDirection.Normalize();
			AddMovementInput(MovementDirection);

			// Checking if AI should jump

			// This section does not fully work, 
			// Which for some reason, even though QueryParams have added AI itself and player character as ignored actor, 
			// The line trace still always return true, even there is nothing between the AI and the player character. 

			UE_LOG(LogTemp, Display, TEXT("Checking if AI can jump now"));
			
			FHitResult HitResult;
			FCollisionQueryParams QueryParams;
			QueryParams.AddIgnoredActor(GetOwner());
			QueryParams.AddIgnoredActor(SensedCharacter);

			// WaistPosition is a new USceneComponent added to BaseCharacter. 
			FVector CurrentCharacterPosition = WaistPosition->GetComponentLocation();

			// Get the bool return from line trace between AI and player character
			isCharacterStuck = (GetWorld()->LineTraceSingleByChannel(HitResult,
				CurrentCharacterPosition,
				LastKnownPlayerLocation,
				ECC_WorldStatic, QueryParams));

			// Draw a debug line to represent the line trace. 
			DrawDebugLine(GetWorld(), CurrentCharacterPosition,
				LastKnownPlayerLocation,
				//FVector(LastKnownPlayerLocation.X, LastKnownPlayerLocation.Y, LastKnownPlayerLocation.Z + 100),
				FColor::Blue, false, 1.0f);

			UE_LOG(LogTemp, Display, TEXT("is waist stuck? %s"), (isCharacterStuck == true) ? TEXT("True") : TEXT("False"));

			if (!isCharacterStuck)
			{
				// This part is suppose to make the AI jump when isCharacterStuck is true. 
				// But since the isCharacterStuck is always true, even when there is nothing between the AI and player character, 
				// this part is disabled by setting the condition as reverse. 
				UE_LOG(LogTemp, Display, TEXT("I am trying to make the AI jump"));
				Super::Jump();
			}
		}
		else{
			LastKnownPlayerLocation = FVector(0, 0, 0);
		}

	}
	else if (!CurrentPath.IsEmpty())
	{
		// If AI does not have directly line of sight on player, 
		// but has a CurrentPath array available to execute path finding, 
		// Follow this path. 

		UE_LOG(LogTemp, Display, TEXT("Moving along CurrentPath"));
		//if (LastKnownPlayerLocation) {
		//	// Move towards the last known player location.
		//	FVector MovementDirection = LastKnownPlayerLocation - GetActorLocation();
		//	MovementDirection.Normalize();
		//	AddMovementInput(MovementDirection);
		//}
		//else if (!CurrentPath.IsEmpty())
		//{
		// 1. Move towards the current stage of the path.
		//		a. Calculate the direction from the current position to the target of the current stage of the path.

		UE_LOG(LogTemp, Display, TEXT("Distance to LastKnownPlayerLocation is: %f"), FVector::Distance(GetActorLocation(), LastKnownPlayerLocation));

		UE_LOG(LogTemp, Display, TEXT("Distance to next node in the CurrentPath is: %f"), FVector::Distance(GetActorLocation(), CurrentPath[CurrentPath.Num() - 1]));

		FVector MovementDirection = CurrentPath[CurrentPath.Num() - 1] - GetActorLocation();
		MovementDirection.Normalize();
		//		b. Apply movement in that direction.
		AddMovementInput(MovementDirection);
		//SetActorRotation(MovementDirection);
		//SetActorRotation(FRotator(MovementDirection.X, MovementDirection.Y, MovementDirection.Z));
		// 2. Check if it is close to the current stage of the path then pop it off.
		if (FVector::Distance(GetActorLocation(), CurrentPath[CurrentPath.Num() - 1]) < PathfindingError)
		{
			// Draw a debug sphere to indicate that AI has reached to this location. 
			DrawDebugSphere(GetWorld(),
				FVector(CurrentPath[CurrentPath.Num() - 1].X, CurrentPath[CurrentPath.Num() - 1].Y, CurrentPath[CurrentPath.Num() - 1].Z + 50),
				50.0f, 4, FColor::Red, true, -1, 0, 5.0f);

			/*if (FVector::Distance(GetActorLocation(), LastKnownPlayerLocation) < PathfindingError)
			{
				LastKnownPlayerLocation = FVector(0, 0, 0);
			}*/

			CurrentPath.Pop();
		}
		//}

		if (CurrentPath.Num() == 0)
		{
			// If AI just finished a CurrentPath, reset the LastKnownPlayerLocation.
			LastKnownPlayerLocation = FVector(0, 0, 0);
		}

	}
	  
}// end of: MoveAlongPath()

void AEnemyCharacter::TickPatrol()
{
	//UE_LOG(LogTemp, Display, TEXT("Now path is empty, generating new patrol path"));
	
	if (CurrentPath.IsEmpty())
	{
		if (LastKnownPlayerLocation != FVector(0, 0, 0))
		{
			// If AI have a LastKnownPlayerLocation, get a path to that location using pathfinding subsystem
			CurrentPath = PathfindingSubsystem->GetPath(GetActorLocation(), LastKnownPlayerLocation);
		}
		else
		{
			//CurrentPath = PathfindingSubsystem->GetRandomPath(GetActorLocation());

			// Obtain a patrol path that prioritise unvisited nodes on the map. 
			// GetPatrolPath(StartingLocation) is a new function in pathfinding subsystem for above functionality. 
			CurrentPath = PathfindingSubsystem->GetPatrolPath(GetActorLocation());
		}
		
	}
	MoveAlongPath();
}

void AEnemyCharacter::TickEngage()
{	
	//if (!SensedCharacter) return;
	if (!SensedCharacter)// If AI has no sight on player
	{
		UE_LOG(LogTemp, Display, TEXT("AI have no sight on player"));
		if (LastKnownPlayerLocation != FVector(0, 0, 0)) 
		{			
			UE_LOG(LogTemp, Display, TEXT("AI have a LastKnownPlayerLocation, getting a path towards that location using Pathfinding subsystem"));
			if (CurrentPath.IsEmpty())
			{
				CurrentPath = PathfindingSubsystem->GetPath(GetActorLocation(), LastKnownPlayerLocation);
			}
			/*CurrentPath.Empty();
			CurrentPath = PathfindingSubsystem->GetPath(GetActorLocation(), LastKnownPlayerLocation);*/
		}
		else 
		{
			UE_LOG(LogTemp, Display, TEXT("No valid path, terminating TickEngage()"));
			return;
		}
	}
	
	/*if (CurrentPath.IsEmpty())
	{
		CurrentPath = PathfindingSubsystem->GetPath(GetActorLocation(), SensedCharacter->GetActorLocation());
	}*/

	MoveAlongPath();

	if (HasWeapon())
	{
		if (WeaponComponent->IsMagazineEmpty())
		{
			Reload();
		}

		if (SensedCharacter) Fire(SensedCharacter->GetActorLocation());
	}
}

void AEnemyCharacter::TickEvade()
{
	// Find the player and return if it can't find it.
	if (!SensedCharacter) return;

	if (CurrentPath.IsEmpty())
	{
		CurrentPath = PathfindingSubsystem->GetPathAway(GetActorLocation(), SensedCharacter->GetActorLocation());
	}
	MoveAlongPath();

	// Enemy AI can still engage with player while evading
	if (HasWeapon())
	{
		if (WeaponComponent->IsMagazineEmpty())
		{
			Reload();
		}
		Fire(SensedCharacter->GetActorLocation());
	}
}

void AEnemyCharacter::OnSensedPawn(APawn* SensedActor)
{
	if (APlayerCharacter* Player = Cast<APlayerCharacter>(SensedActor))
	{
		SensedCharacter = Player;
		//UE_LOG(LogTemp, Display, TEXT("Sensed Player"))
		/*LastKnownPlayerLocation = SensedCharacter->GetActorLocation();*/
	}
}

void AEnemyCharacter::UpdateSight()
{
	if (!SensedCharacter) return;
	if (PawnSensingComponent)
	{
		if (!PawnSensingComponent->HasLineOfSightTo(SensedCharacter))
		{
			SensedCharacter = nullptr;
			//UE_LOG(LogTemp, Display, TEXT("Lost Player"))
		}
	}
}


// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateSight();

	//UE_LOG(LogTemp, Display, TEXT("AI doesn't have a CurrentPath? %s"), (CurrentPath.Num() == 0) ? TEXT("True") : TEXT("False"));

	if (SensedCharacter)
	{
		// Update the LastKnownPlayerLocation once AI have sight on player
		LastKnownPlayerLocation = SensedCharacter->GetActorLocation();
		//CurrentPath.Empty();
	}

	UE_LOG(LogTemp, Warning, TEXT("LastKnownPlayerLocation is %f, %f, %f"),
		LastKnownPlayerLocation.X,
		LastKnownPlayerLocation.Y,
		LastKnownPlayerLocation.Z);
	
	//UE_LOG(LogTemp, Display, TEXT("AI's SensedCharacter is nullptr? %s"), (SensedCharacter == nullptr) ? TEXT("True") : TEXT("False"));
	
	switch(CurrentState)
	{
	case EEnemyState::Patrol:
		TickPatrol();
		if (SensedCharacter)
		{
			if (HealthComponent->GetCurrentHealthPercentage() >= 0.4f)
			{
				CurrentState = EEnemyState::Engage;
			} else
			{
				CurrentState = EEnemyState::Evade;
			}
			CurrentPath.Empty();
		}
		break;
	case EEnemyState::Engage:
		TickEngage();
		if (HealthComponent->GetCurrentHealthPercentage() < 0.4f)
		{
			CurrentPath.Empty();
			CurrentState = EEnemyState::Evade;
		} else if (!SensedCharacter && LastKnownPlayerLocation == FVector(0, 0, 0))
		{
			//CurrentPath.Empty();
			CurrentState = EEnemyState::Patrol;
		}
		break;
	case EEnemyState::Evade:
		TickEvade();
		if (HealthComponent->GetCurrentHealthPercentage() >= 0.4f)
		{
			CurrentPath.Empty();
			CurrentState = EEnemyState::Engage;
		} else if (!SensedCharacter)
		{
			CurrentState = EEnemyState::Patrol;
		}
		break;
	}

	

}

// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

APlayerCharacter* AEnemyCharacter::FindPlayer() const
{
	APlayerCharacter* Player = nullptr;
	for (TActorIterator<APlayerCharacter> It(GetWorld()); It; ++It)
	{
		Player = *It;
		break;
	}
	if (!Player)
	{
		UE_LOG(LogTemp, Error, TEXT("Unable to find the Player Character in the world."))
	}
	return Player;


	
}

