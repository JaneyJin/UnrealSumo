// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Runtime/XmlParser/Public/FastXml.h"
#include "Runtime/XmlParser/Public/XmlParser.h"
#include <vector>
#include "ContainersAndTypes.h"


class SimpleNode;

class UfileParser : public IFastXmlCallback
{

private:
	// Constructor
	FString selectedXMLFile;
	
	UWorld* World = GEditor->GetEditorWorldContext().World();

	// Process Elements
	bool isElementNode = false;
	bool isElementEdge = false;

	// InitializeNodeAttributes
	FString tempNodeID = "";
	bool isPriorityNode = false;
	const TCHAR* nodeXCoordinate;
	const TCHAR* nodeYCoordinate;
	bool xCoordinateIsSet = false;
	bool yCoordinateIsSet = false;

	// InitializeEdgeAttributes
	FString tempEdgeID = "";
	FString fromNode = "";
	FString toNode = "";
	FString laneLength = "";
	bool fromNodeSet = false;
	bool toNodeSet = false;
	bool lengthIsSet = false;
	bool shapeIsSet = false;

	// ShapeProcessing
	std::vector<float> Shapecoordinates;

	// InitializeNodeAttributes
	///Put the shape coordinates into Shapecoordinates vector
	void ShapeProcessing(const TCHAR*);

	// InitializeNode
	NodeContainer NodeCont;




public:
	
	UfileParser(const TCHAR*);
	virtual ~UfileParser();

	bool loadxml();

	// Override original function
	bool ProcessXmlDeclaration(const TCHAR* ElementData, int32 XmlFileLineNumber) override;
	bool ProcessElement(const TCHAR* ElementName, const TCHAR* ElementData, int32 XmlFileLineNumber) override;
	bool ProcessAttribute(const TCHAR* AttributeName, const TCHAR* AttributeValue) override;
	bool ProcessClose(const TCHAR* Element) override;
	bool ProcessComment(const TCHAR* Comment) override;

	void InitializeNodeAttributes(const TCHAR* AttributeName, const TCHAR* AttributeValue);
	void InitializeEdgeAttributes(const TCHAR* AttributeName, const TCHAR* AttributeValue);
	SimpleNodePtr InitializeNode();
	
	// To find all actors that are the types ARoadMesh, AAtmosphericFog and ASkyLight types, and destroy them
	void destroyFoundActors();
	void getAllActorsOfClass();
};