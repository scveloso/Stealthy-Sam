#pragma once

#include "common.hpp"
#include <vector>


class DecisionNode
{
public:
	DecisionNode(int action);
	DecisionNode(int trueNextNode, int falseNextNode);

	int getAction();
	int getNextNode(bool satisfiesCondition);

	bool m_isEndNode;
	int m_trueNextNode;
	int m_falseNextNode;

	int m_action;
};
