// Fill out your copyright notice in the Description page of Project Settings.


#include "fileParser.h"
#include "Engine.h"
#include "Editor.h" // For GEditor
#include "Runtime/XmlParser/Public/XmlParser.h" // Import XML function
#include <sstream> // std::string
#include "SimpleNode.h" 
#include "NetMesh.h" // TODO : include in SimpleEdge

//Initializer List
UfileParser::UfileParser(const TCHAR* selectedFile) : selectedXMLFile(selectedFile)
{
	
	// Set AtomosphericFog
	FVector Location = FVector(0.0f, 0.0f, 100.0f);
	FRotator Rotation = FRotator(0.0f, 0.0f, 0.0f);
	FActorSpawnParameters SpawnParameters;
	World->SpawnActor<AAtmosphericFog>(Location, Rotation, SpawnParameters);
	Location.Z = 100000.0f;

	// ???
	ASkyLight* Skylight = World->SpawnActor<ASkyLight>(Location, Rotation, SpawnParameters);
	
	if (Skylight != nullptr) {
		/* FLightingBuildOptions LightOptions;
		Skylight->GetLightComponent()->SetIntensity(5.0f);
		GEditor->BuildLighting(LightOptions);*/
	}
}

void UfileParser::getAllActorsOfClass() {
	////UGameplayStatics::GetAllActorsOfClass(World, ARoadMesh::StaticClass(), FoundActors);
	//// destroyFoundActors();
	//UGameplayStatics::GetAllActorsOfClass(World, AAtmosphericFog::StaticClass(), FoundActors);
	//destroyFoundActors();
	//UGameplayStatics::GetAllActorsOfClass(World, ASkyLight::StaticClass(), FoundActors);
	//destroyFoundActors();
	//// UGameplayStatics::GetAllActorsOfClass(World, AStopSignMesh::StaticClass(), FoundActors);
	//// destroyFoundActors();
}
void UfileParser::destroyFoundActors() {
	//for (int i = 0; i < FoundActors.Num(); i++) {
	//	World->DestroyActor(FoundActors[i]); //Destroy all actors before starting
	//}
}

UfileParser::~UfileParser()
{
}

bool UfileParser::loadxml()
{
	UE_LOG(LogEngine, Warning, TEXT("Loading started"));
	FText outError;
	int32 outErrorNum;
	
	bool success = FFastXml::ParseXmlFile((IFastXmlCallback*)(this), selectedXMLFile.GetCharArray().GetData(), TEXT(""), nullptr, false, false, outError, outErrorNum);
	return success;
	
}




bool UfileParser::ProcessXmlDeclaration(const TCHAR* ElementData, int32 XmlFileLineNumber)
{
	 UE_LOG(LogEngine, Warning, TEXT("ProcessXmlDeclaration ElementData: %s, XmlFileLineNumber: %f"), ElementData, XmlFileLineNumber);
	return true;
}

bool UfileParser::ProcessElement(const TCHAR* ElementName, const TCHAR* ElementData, int32 XmlFileLineNumber)
{
	if ((FString(ElementName)).Equals(TEXT("junction")))
	{
		isElementNode = true;
		isElementEdge = false;
		UE_LOG(LogEngine, Warning, TEXT("ProcessElement ElementName: %s, ElementValue: %s"), ElementName, ElementData);
	}
	else if ((FString(ElementName)).Equals(TEXT("edge")))
	{
		isElementEdge = true;
		isElementNode = false;
		UE_LOG(LogEngine, Warning, TEXT("ProcessElement ElementName: %s, ElementValue: %s"), ElementName, ElementData);
	}
	else
	{
		isElementNode = false;
	}
	
	return true;
}

bool UfileParser::ProcessAttribute(const TCHAR* AttributeName, const TCHAR* AttributeValue)
{

	if (isElementNode == true)
	{
		UE_LOG(LogEngine, Warning, TEXT("ProcessAttribute AttributeName: %s, AttributeValue: %s"), AttributeName, AttributeValue);
		InitializeNodeAttributes(AttributeName, AttributeValue);
		if ((shapeIsSet == true) && (xCoordinateIsSet == true) && (yCoordinateIsSet == true))
		{
			InitializeNode();
			//UE_LOG(LogEngine, Warning, TEXT("Node object created!"));
		}

	}
	else if (isElementEdge == true)
	{
		UE_LOG(LogEngine, Warning, TEXT("ProcessAttribute AttributeName: %s, AttributeValue: %s"), AttributeName, AttributeValue);
		InitializeEdgeAttributes(AttributeName, AttributeValue);


	}


	return true;
}

bool UfileParser::ProcessClose(const TCHAR* Element)
{
	
	UE_LOG(LogEngine, Warning, TEXT("ProcessClose Element %s"), Element);
	return true;
}

bool UfileParser::ProcessComment(const TCHAR* Comment)
{
	// UE_LOG(LogEngine, Warning, TEXT("ProcessComment Comment: %s"), Comment);
	return true;
}



void UfileParser::InitializeNodeAttributes(const TCHAR* AttributeName, const TCHAR* AttributeValue) {
	if (FString(AttributeName).Equals(TEXT("id")))
	{
		tempNodeID = FString(AttributeValue);
		return;
	}

	if (((FString(AttributeValue)).Equals(TEXT("priority"))))
	{
		isPriorityNode = true;
		return;
	}

	//Set temp node positions
	if ((isPriorityNode == true) && ((FString(AttributeName)).Equals(TEXT("x"))))
	{
		nodeXCoordinate = AttributeValue;
		xCoordinateIsSet = true;
		return;
	}else if ((isPriorityNode == true) && ((FString(AttributeName)).Equals(TEXT("y"))))
	{
		nodeYCoordinate = AttributeValue;
		yCoordinateIsSet = true;
		return;
	}else if ((isPriorityNode == true) && ((FString(AttributeName)).Equals(TEXT("shape"))))
	{
		ShapeProcessing(AttributeValue);
		shapeIsSet = true;
		return;
	}else
	{
		return;
	}
}

void UfileParser::InitializeEdgeAttributes(const TCHAR* AttributeName, const TCHAR* AttributeValue) {
	if (FString(AttributeName).Equals(TEXT("id")))
	{
		tempEdgeID = FString(AttributeValue);
		return;
	}
	else if (FString(AttributeName).Equals(TEXT("from")))
	{
		fromNode = FString(AttributeValue);
		fromNodeSet = true;
		return;
	}
	else if (FString(AttributeName).Equals(TEXT("to")))
	{
		toNode = FString(AttributeValue);
		toNodeSet = true;
		return;
	}
	else if (FString(AttributeName).Equals(TEXT("length")))
	{
		laneLength = FString(AttributeValue);
		lengthIsSet = true;
		return;
	}
	else if (FString(AttributeName).Equals(TEXT("shape")))
	{
		ShapeProcessing(AttributeValue);
		shapeIsSet = true;
		return;
	}
	else
	{
		return;
	}
}

void UfileParser::ShapeProcessing(const TCHAR* ShapeString)
{
	std::string CoordinateString = TCHAR_TO_UTF8(ShapeString);
	Shapecoordinates.clear();
	// Iterator each character
	for (std::string::iterator it = CoordinateString.begin(); it != CoordinateString.end(); ++it)
	{
		if (*it == ',')
		{
			*it = ' ';
		}
		else continue;			//convert all commas in string stream into space
	}

	std::stringstream ss;
	ss << CoordinateString;
	float found;
	int i = 0;
	// extraction operator
	while (!ss.eof())
	{
		//check if it is valid to put stringstream object into float variable
		/*if (ss >> found)
		{
			Shapecoordinates.push_back(found);
		}*/

		//check if it is valid to put stringstream object into float variable. Also check for every second index - if found multiply with with negative 1 to mirror about y axis.
		if (ss >> found) {
			if ((i % 2) == 0) {
				found = (-1) * found; //mirror the network about the x-axis. This means changing the sign of the y coordinate. 
				Shapecoordinates.push_back(found * 100); //Since the default unreal engine unit is cm and the default SUMO unit is m, we perform the conversion here.
				i++;
			}
			else {
				// Unit conversion
				Shapecoordinates.push_back(found * 100);
				i++;
			}
		}
	}

}


SimpleNodePtr UfileParser::InitializeNode() {
	// std::unique_ptr<SimpleNode> declared in ContainersAndTypes
	// Pointer to the managed object or nullptr if no object is owned
	SimpleNodePtr Node = std::make_unique<SimpleNode>();
	Node->SetID(*tempNodeID);
	// std::vector<float> 
	// Unit is meter here. Need to switch to Unreal unit
	// Node->SetShapeCoordinates(Shapecoordinates);
	Node->SetPosition(nodeXCoordinate, nodeYCoordinate);


	//initialize map with the pointer for extended node lifetime
	NodeCont.NodeMap.Add(*tempNodeID, Node.get());

	// FTransform(const FQuat& InRotation,const FVector& InTranslation,const FVector& InScale3D=FVector::OneVector) 
	// Transformation of position vectors is applied in the order: Scale -> Rotate -> Translate.
	// One vector (1,1,1)
	FQuat rotation(0.0f, 0.0f, 0.0f, 0.0f);
	FVector translation(Node->NodePosition.X, Node->NodePosition.Y, 0.0f);
	FTransform transform(rotation,translation);

	// Returns the WorldContext for the editor world.
	UWorld* World = GEditor->GetEditorWorldContext().World();
	// Spawns an instance of an actor class, but does not automatically run its construction script.
	 ANetMesh*  MyDeferredActor = Cast<ANetMesh>(UGameplayStatics::BeginDeferredActorSpawnFromClass(World, ANetMesh::StaticClass(), transform)); //Downcasting

	if (MyDeferredActor) {



		// 'Finish' spawning an actor.This will run the construction script.
		UGameplayStatics::FinishSpawningActor(MyDeferredActor, transform);
		UE_LOG(LogEngine, Warning, TEXT("Node actor is spawned."));
	}

	return Node;
}