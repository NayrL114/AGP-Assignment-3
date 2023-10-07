// Fill out your copyright notice in the Description page of Project Settings.


#include "PathfindingSubsystem.h"

#include "../Characters/BaseCharacter.h"
#include "../Characters/PlayerCharacter.h"
#include "../Characters/EnemyCharacter.h"

#include "EngineUtils.h"
#include "NavigationNode.h"

void UPathfindingSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	UE_LOG(LogTemp, Warning, TEXT("Creating the UPathfindingSubsystem."))
	PopulateNodes();
}

TArray<FVector> UPathfindingSubsystem::GetWaypointPositions() const
{
	TArray<FVector> NodePositions;
	for (ANavigationNode* Node : Nodes)
	{
		if (Node)
		{
			NodePositions.Add(Node->GetActorLocation());
		}
	}
	return NodePositions;
}

TArray<FVector> UPathfindingSubsystem::GetRandomPath(const FVector& StartLocation)
{
	return GetPath(FindNearestNode(StartLocation), GetRandomNode());
}

TArray<FVector> UPathfindingSubsystem::GetPath(const FVector& StartLocation, const FVector& TargetLocation)
{
	return GetPath(FindNearestNode(StartLocation), FindNearestNode(TargetLocation));
}

TArray<FVector> UPathfindingSubsystem::GetPathAway(const FVector& StartLocation, const FVector& TargetLocation)
{
	return GetPath(FindNearestNode(StartLocation), FindFurthestNode(TargetLocation));
}

// This function will generate a patrol path, 
// that at least have the end point as a navigation node that has never been reached by the AI. 
TArray<FVector> UPathfindingSubsystem::GetPatrolPath(const FVector& StartLocation)
{
	// Check if the AI had been visited all the nodes on the map. 
	if (PatrolledRoutes.Num() >= Nodes.Num())
	{
		UE_LOG(LogTemp, Display, TEXT("Now AI has visited all the navigagtion nodes on the map. Resetting patrolled routes array. "));
		PatrolledRoutes.Empty();
	}

	// Find a node that AI has never been visited. 
	ANavigationNode* UnvisitedTargetLocation = nullptr;	

	int i = 0;
	TArray<ANavigationNode*> NearestNodes;
	if (FindNearestNode(StartLocation)->ConnectedNodes.Num() != 0) NearestNodes = FindNearestNode(StartLocation)->ConnectedNodes;

	TArray<int> VisitedIndexs = TArray<int>();

	do 
	{
		int a = 0;
		//if (NearestNodes = FindNearestNode(StartLocation)->ConnectedNodes)
		//{
			//if (NearestNodes.Num() != 0 && i < NearestNodes.Num())
		if (i < NearestNodes.Num())
		{
			//UnvisitedTargetLocation = NearestNodes[i];
			do
			{
				a = FMath::RandRange(0, NearestNodes.Num() - 1);				
			} while (VisitedIndexs.Contains(a));

			UnvisitedTargetLocation = NearestNodes[a];
			VisitedIndexs.Add(a);

			i++;
			continue; // Skil the get random node part and continue the larger do section
		}			
		//}
		UnvisitedTargetLocation = GetRandomNode();
	} while (PatrolledRoutes.Contains(UnvisitedTargetLocation->GetActorLocation()));

	// Generate a patrol route with the end point as the never visited node. 
	TArray<FVector> NewPath = GetPath(FindNearestNode(StartLocation), UnvisitedTargetLocation);
		
	// Record the path into patrolled routes node array. 
	for (FVector Node : NewPath)
	{
		if (!PatrolledRoutes.Contains(Node))
		{
			//UE_LOG(LogTemp, Display, TEXT("Adding %f, %f, %f into patrolled routes array"), Node.X, Node.Y, Node.Z);
			PatrolledRoutes.Add(Node);
		}
	}

	// Send the patrol path back to AI. 
	return NewPath;
}

// For now this function is not active at all
void UPathfindingSubsystem::GenerateNodeOnCharacterLocation() 
{
	// This section of code is for removing the navigation node spawned based on character locations
	//if (ProcedurallyPlacedNodes.Num() != 0)
	//{
	//	//ProcedurallyPlacedNodes.Add(Node);
	//	//for (ANavigationNode* Node : CharacterLocations) 
	//	//{
	//		if (ProcedurallyPlacedNodes.Contains(Node))
	//		{
	//			Node->RemoveNodeConnections();
	//			ProcedurallyPlacedNodes.Remove(Node);
	//		}
	//	//}
	//}

	//if (Nodes.Num() != 0)
	//{
	//	//for (ANavigationNode* Node : CharacterLocations)
	//	//{
	//		if (Nodes.Contains(Node))
	//		{
	//			Node->RemoveNodeConnections();
	//			Nodes.Remove(Node);
	//		}
	//	//}
	//}

	//CharacterLocations.Empty();

	CharacterLocations.Empty();

	// Than generate nodes on all base character's location. 
	//GenerateNodeOnCharacterLocation();
	for (TActorIterator<ABaseCharacter> It(GetWorld()); It; ++It)
	{
		//It->GetActorLocation();
		//Nodes.Add(*It);
		//UE_LOG(LogTemp, Warning, TEXT("NODE: %s"), *(*It)->GetActorLocation().ToString())
		if (ANavigationNode* Node = GetWorld()->SpawnActor<ANavigationNode>())
		{
			Node->SetActorLocation(It->GetActorLocation());

			//Node->AddNodeConnection(FindNearestNode(It->GetActorLocation()));

			CharacterLocations.Add(Node);

			//ProcedurallyPlacedNodes.Add(Node);

			/*if (ProcedurallyPlacedNodes.Num() != 0)
			{
				ProcedurallyPlacedNodes.Add(Node);
			}*/

			/*if (Nodes.Num() != 0)
			{
				Nodes.Add(Node);
			}*/

		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Unable to spawn a node on base character location in GenerateNodeOnCharacterLocation. This is bad!"))
		}
	}

	//if (CharacterLocations.Num() == 0) 
	//{
	//	return;
	//}

	//int i = 0;

	//// Find all BaseCharacter actors on the level, and spawn a navigation node based on their location. 
	//for (TActorIterator<ABaseCharacter> It(GetWorld()); It; ++It)
	//{
	//	UE_LOG(LogTemp, Display, TEXT("Now there are %d BaseCharacters on the field."), CharacterLocations.Num());
	//	if (i < CharacterLocations.Num()) 
	//	{
	//		CharacterLocations[i]->SetActorLocation(It->GetActorLocation());			
	//	}
	//	i++;
	//	
	//	//It->GetActorLocation();
	//	//Nodes.Add(*It);
	//	//UE_LOG(LogTemp, Warning, TEXT("NODE: %s"), *(*It)->GetActorLocation().ToString())
	//	//if (ANavigationNode* Node = GetWorld()->SpawnActor<ANavigationNode>())

	//	//{
	//	/*	Node->SetActorLocation(It->GetActorLocation());

	//		Node->AddNodeConnection(FindNearestNode(It->GetActorLocation()));
	//		
	//		CharacterLocations.Add(Node);

	//		if (ProcedurallyPlacedNodes.Num() != 0) 
	//		{
	//			ProcedurallyPlacedNodes.Add(Node);
	//		}

	//		if (Nodes.Num() != 0)
	//		{
	//			Nodes.Add(Node);
	//		}

	//	}
	//	else
	//	{
	//		UE_LOG(LogTemp, Error, TEXT("Unable to spawn a node on base character location. This is bad!"))
	//	}*/
	//}
}// end of: GenerateNodeOnCharacterLocation()

void UPathfindingSubsystem::PlaceProceduralNodes(const TArray<FVector>& LandscapeVertexData, int32 MapWidth, int32 MapHeight)
{
	// Need to destroy all of the current nodes in the world.
	RemoveAllNodes();

	// Then create and place all the nodes and store them in the ProcedurallyPlacedNodes array.
	for (int Y = 0; Y < MapHeight; Y++)
	{
		for (int X = 0; X < MapWidth; X++)
		{
			// Spawn the node in
			if (ANavigationNode* Node = GetWorld()->SpawnActor<ANavigationNode>())
			{
				Node->SetActorLocation(LandscapeVertexData[Y * MapWidth + X]);
				ProcedurallyPlacedNodes.Add(Node);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Unable to spawn a node for some reason. This is bad!"))
			}

		}
	}

	//CharacterLocations.Empty();

	//// Than generate nodes on all base character's location. 
	////GenerateNodeOnCharacterLocation();
	//for (TActorIterator<ABaseCharacter> It(GetWorld()); It; ++It)
	//{
	//	//It->GetActorLocation();
	//	//Nodes.Add(*It);
	//	//UE_LOG(LogTemp, Warning, TEXT("NODE: %s"), *(*It)->GetActorLocation().ToString())
	//	if (ANavigationNode* Node = GetWorld()->SpawnActor<ANavigationNode>())
	//	{
	//		Node->SetActorLocation(It->GetActorLocation());

	//		//Node->AddNodeConnection(FindNearestNode(It->GetActorLocation()));

	//		CharacterLocations.Add(Node);

	//		ProcedurallyPlacedNodes.Add(Node);

	//		/*if (ProcedurallyPlacedNodes.Num() != 0)
	//		{
	//			ProcedurallyPlacedNodes.Add(Node);
	//		}*/

	//		/*if (Nodes.Num() != 0)
	//		{
	//			Nodes.Add(Node);
	//		}*/

	//	}
	//	else
	//	{
	//		UE_LOG(LogTemp, Error, TEXT("Unable to spawn a node on base character location when generating. This is bad!"))
	//	}
	//}


	// Then add connections between all adjacent nodes.
	for (int Y = 0; Y < MapHeight; Y++)
	{
		for (int X = 0; X < MapWidth; X++)
		{
			if (ANavigationNode* CurrentNode = ProcedurallyPlacedNodes[Y * MapWidth + X]) // Make sure it's a valid ptr.
			{
				// ADD CONNECTIONS:
				// Add Left
				if (X != MapWidth-1)
					CurrentNode->ConnectedNodes.Add(ProcedurallyPlacedNodes[Y * MapWidth + X+1]);
				// Add Up
				if (Y != MapHeight-1)
					CurrentNode->ConnectedNodes.Add(ProcedurallyPlacedNodes[(Y+1) * MapWidth + X]);
				// Add Right
				if (X != 0)
					CurrentNode->ConnectedNodes.Add(ProcedurallyPlacedNodes[Y * MapWidth + X-1]);
				// Add Down
				if (Y != 0)
					CurrentNode->ConnectedNodes.Add(ProcedurallyPlacedNodes[(Y-1) * MapWidth + X]);
				// Add UpLeft
				if (X != MapWidth-1 && Y != MapHeight-1)
					CurrentNode->ConnectedNodes.Add(ProcedurallyPlacedNodes[(Y+1) * MapWidth + X+1]);
				// Add UpRight
				if (X != 0 && Y != MapHeight-1)
					CurrentNode->ConnectedNodes.Add(ProcedurallyPlacedNodes[(Y+1) * MapWidth + X-1]);
				// Add DownRight
				if (X != 0 && Y != 0)
					CurrentNode->ConnectedNodes.Add(ProcedurallyPlacedNodes[(Y-1) * MapWidth+ X-1]);
				// Add DownLeft
				if (X != MapWidth-1 && Y != 0)
					CurrentNode->ConnectedNodes.Add(ProcedurallyPlacedNodes[(Y-1) * MapWidth + X+1]);
			}
		}
	}
}

void UPathfindingSubsystem::PopulateNodes()
{
	Nodes.Empty();

	for (TActorIterator<ANavigationNode> It(GetWorld()); It; ++It)
	{
		Nodes.Add(*It);
		//UE_LOG(LogTemp, Warning, TEXT("NODE: %s"), *(*It)->GetActorLocation().ToString())
	}
}

void UPathfindingSubsystem::RemoveAllNodes()
{
	Nodes.Empty();
	ProcedurallyPlacedNodes.Empty();

	for (TActorIterator<ANavigationNode> It(GetWorld()); It; ++It)
	{
		GetWorld()->DestroyActor(*It);
	}
}

ANavigationNode* UPathfindingSubsystem::GetRandomNode()
{
	// Failure condition
	if (Nodes.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("The nodes array is empty."))
		return nullptr;
	}
	const int32 RandIndex = FMath::RandRange(0, Nodes.Num()-1);
	return Nodes[RandIndex];
}

ANavigationNode* UPathfindingSubsystem::FindNearestNode(const FVector& TargetLocation)
{
	// Failure condition.
	if (Nodes.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("The nodes array is empty."))
		return nullptr;
	}

	// Using the minimum programming pattern to find the closest node.
	// What is the Big O complexity of this? Can you do it more efficiently?
	ANavigationNode* ClosestNode = nullptr;
	float MinDistance = UE_MAX_FLT;
	for (ANavigationNode* Node : Nodes)
	{
		const float Distance = FVector::Distance(TargetLocation, Node->GetActorLocation());
		if (Distance < MinDistance)
		{
			MinDistance = Distance;
			ClosestNode = Node;
		}
	}

	return ClosestNode;
}

ANavigationNode* UPathfindingSubsystem::FindFurthestNode(const FVector& TargetLocation)
{
	// Failure condition.
	if (Nodes.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("The nodes array is empty."))
		return nullptr;
	}

	// Using the minimum programming pattern to find the closest node.
	// What is the Big O complexity of this? Can you do it more efficiently?
	ANavigationNode* FurthestNode = nullptr;
	float MaxDistance = -1.0f;
	for (ANavigationNode* Node : Nodes)
	{
		const float Distance = FVector::Distance(TargetLocation, Node->GetActorLocation());
		if (Distance > MaxDistance)
		{
			MaxDistance = Distance;
			FurthestNode = Node;
		}
	}

	return FurthestNode;
}

TArray<FVector> UPathfindingSubsystem::GetPath(ANavigationNode* StartNode, ANavigationNode* EndNode)
{
	if (!StartNode || !EndNode)
	{
		UE_LOG(LogTemp, Error, TEXT("Either the start or end node are nullptrs."))
		return TArray<FVector>();
	}

	// Setup the open set and add the start node.
	TArray<ANavigationNode*> OpenSet;
	OpenSet.Add(StartNode);

	// StartNode->GScore = UE_MAX_FLT;
	// StartNode->HScore = FVector::Distance(StartNode->GetActorLocation(), EndNode->GetActorLocation());

	// Setup the maps that will hold the GScores, HScores and CameFrom
	TMap<ANavigationNode*, float> GScores, HScores;
	TMap<ANavigationNode*, ANavigationNode*> CameFrom;
	// You could pre-populate the GScores and HScores maps with all of the GScores (at infinity) and HScores here by looping over
	// all the nodes in the Nodes array. However it is more efficient to only calculate these when you need them
	// as some nodes might not be explored when finding a path.

	// Setup the start nodes G and H score.
	GScores.Add(StartNode, 0);
	HScores.Add(StartNode, FVector::Distance(StartNode->GetActorLocation(), EndNode->GetActorLocation()));
	CameFrom.Add(StartNode, nullptr);

	while (!OpenSet.IsEmpty())
	{
		// Find the node in the open set with the lowest FScore.
		ANavigationNode* CurrentNode = OpenSet[0]; // We know this exists because the OpenSet is not empty.
		for (int32 i = 1; i < OpenSet.Num(); i++)
		{
			// We can be sure that all the nodes in the open set have already had their GScores and HScores calculated.
			if (GScores[OpenSet[i]] + HScores[OpenSet[i]] < GScores[CurrentNode] + HScores[CurrentNode])
			{
				CurrentNode = OpenSet[i];
			}
		}

		// Remove the current node from the OpenSet
		OpenSet.Remove(CurrentNode);

		if (CurrentNode == EndNode)
		{
			// Then we have found the path so reconstruct it and get the positions of each of the nodes in the path.
			// UE_LOG(LogTemp, Display, TEXT("PATH FOUND"))
			return ReconstructPath(CameFrom, EndNode);
		}

		for (ANavigationNode* ConnectedNode : CurrentNode->ConnectedNodes)
		{
			if (!ConnectedNode) continue; // Failsafe if the ConnectedNode is a nullptr.
			const float TentativeGScore = GScores[CurrentNode] + FVector::Distance(CurrentNode->GetActorLocation(), ConnectedNode->GetActorLocation());
			// Because we didn't setup all the scores and came from at the start, we need to check if the connected node has a gscore
			// already otherwise set it. If it doesn't have a gscore then it won't have all the other things either so initialise them as well.
			if (!GScores.Contains(ConnectedNode))
			{
				GScores.Add(ConnectedNode, UE_MAX_FLT);
				HScores.Add(ConnectedNode, FVector::Distance(ConnectedNode->GetActorLocation(), EndNode->GetActorLocation()));
				CameFrom.Add(ConnectedNode, nullptr);
			}

			// Then update this nodes scores and came from if the tentative g score is lower than the current g score.
			if (TentativeGScore < GScores[ConnectedNode])
			{
				CameFrom[ConnectedNode] = CurrentNode;
				GScores[ConnectedNode] = TentativeGScore;
				// HScore is already set when adding the node to the HScores map.
				// Then add connected node to the open set if it isn't already in there.
				if (!OpenSet.Contains(ConnectedNode))
				{
					OpenSet.Add(ConnectedNode);
				}
			}
		}
	}

	// If we get here, then no path has been found so return an empty array.
	return TArray<FVector>();
	
}

TArray<FVector> UPathfindingSubsystem::ReconstructPath(const TMap<ANavigationNode*, ANavigationNode*>& CameFromMap, ANavigationNode* EndNode)
{
	TArray<FVector> NodeLocations;

	const ANavigationNode* NextNode = EndNode;
	while(NextNode)
	{
		NodeLocations.Push(NextNode->GetActorLocation());
		NextNode = CameFromMap[NextNode];
	}

	return NodeLocations;
}

