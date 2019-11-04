#pragma once
#include "ContainersAndTypes.h"
#include "Engine.h" // TCHAR
#include <vector>

#define UNITCONVERSION 100
#define YCONVERSION -1

class SimpleNode
{
	friend class UfileParser;

private:
	FString NodeID = "";
	std::vector<float> NodeShapeCoordinates;
	FVector NodePosition; // Pos(x,y,z)

public:
	void SetID(const TCHAR*);
	void SetPosition(const TCHAR*, const TCHAR*);
	void SetShapeCoordinates(std::vector<float> Shapecoordinates);

};