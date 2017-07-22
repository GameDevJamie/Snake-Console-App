#pragma once

#include "SCoords.h"
#include "CSnake.h"

struct SNode
{
	SCoords coords;
	int cost;	//Total cost to move to this node
	int total;	//Cost + Heuristic

	EDirection direction;

	SNode* parent;

	SNode(SCoords newCoords, int newCost)
	{
		coords = newCoords;
		cost = newCost;
		parent = NULL;

		direction = EDirection::RIGHT;
	}
};