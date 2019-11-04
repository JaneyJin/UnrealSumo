#include "SimpleNode.h"

void SimpleNode::SetID(const TCHAR * tempID)
{
	NodeID = FString(tempID);
	UE_LOG(LogEngine, Warning, TEXT("New node ID is %s"), tempID);
}

void SimpleNode::SetPosition(const TCHAR* xCoordinate, const TCHAR* yCoordinate)
{	
	// Modified
	NodePosition.X = FCString::Atof(xCoordinate) * UNITCONVERSION;
	NodePosition.Y = FCString::Atof(yCoordinate) * UNITCONVERSION * YCONVERSION;
	UE_LOG(LogEngine, Warning, TEXT("New node coordinates are %s %s"), xCoordinate, yCoordinate);
}


void SimpleNode::SetShapeCoordinates(std::vector<float> Shapecoordinates) {
	for (int i = 0; i < Shapecoordinates.size();i++) {
		if (i % 2 == 1) {
			NodeShapeCoordinates.push_back(Shapecoordinates[i] * UNITCONVERSION * YCONVERSION);
		}
		else {
			NodeShapeCoordinates.push_back(Shapecoordinates[i] * UNITCONVERSION);
		}
	}
}