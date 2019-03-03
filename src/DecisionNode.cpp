#include "DecisionNode.hpp"

// Represents a node in a decision tree
// Can either be a decision node or an end node
// Decision nodes lead to other nodes based on a condition
// End nodes have an action that should be executed when reached

// ** END NODES **
// An end node in the decision tree
DecisionNode::DecisionNode(int action) {
  m_isEndNode = true;
  m_action = action;
}

int DecisionNode::getAction() {
  if (m_isEndNode) {
    return m_action;
  }

  return -1;
}

// ** DECISION NODES **
// A decision node in the decision tree
DecisionNode::DecisionNode(int trueNextNode, int falseNextNode) {
  m_isEndNode = false;
  m_trueNextNode = trueNextNode;
  m_falseNextNode = falseNextNode;
}

int DecisionNode::getNextNode(bool satisfiesCondition) {
  if (satisfiesCondition) {
    return m_trueNextNode;
  }

  return m_falseNextNode;
}
