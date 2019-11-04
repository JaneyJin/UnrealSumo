#pragma once
#include <memory> //unique_ptr
#include "Map.h"

class SimpleNode;


// Shared pointer for every node object created
// Create an alias or new name for an existing type or user defined type
typedef std::unique_ptr<SimpleNode> SimpleNodePtr; 


//Node Map
typedef TMap<const TCHAR*, SimpleNode*> NodeTMap;
class NodeContainer {

public:
	//Maps to store the pointers to the parsed nodes (pointers to the objects created)
	NodeTMap NodeMap;
};



