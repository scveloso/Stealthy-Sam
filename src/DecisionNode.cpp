#include "DecisionNode.hpp"

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
