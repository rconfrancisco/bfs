#include <iostream>
#include <queue>
#include <memory>

/*Sample program to do a breadth first search and set pointers on each level
     1
   2   3
 4  5 6  7
     so:
 7->6->5->4->nil
 3->2->nil
 1->nil
*/
struct Node_t;
using NodePtr_t   = std::shared_ptr<Node_t>;
using WeakPtr_t   = std::weak_ptr<Node_t>;
using NodeQueue_t = std::queue<NodePtr_t>;

struct Node_t{
  int        data;
  NodePtr_t  left;
  NodePtr_t  right;
  WeakPtr_t  previous;
  size_t     level;
  Node_t () : data(0), level(0) {};
};


void insertNode(NodePtr_t& node, NodeQueue_t& myQueue, const size_t level) {
  if(node.get()) {
    node.get()->level    = level;
    myQueue.push(node);
  }
}

void updateTree(NodePtr_t& root) {
  if(!(root.get())) return;
  NodeQueue_t myQueue;
  insertNode(root, myQueue, 0);

  NodePtr_t previous, currNode;
  size_t lastLevel =0;
  while(!myQueue.empty()) {
    currNode = myQueue.front();
    myQueue.pop();
    size_t level = currNode.get()->level; 
    if(level != lastLevel) {
      previous.reset();
      lastLevel = level;
    }
    currNode.get()->previous = previous;
    previous = currNode;
    insertNode(currNode.get()->left,  myQueue, level+1);
    insertNode(currNode.get()->right, myQueue, level+1);
  }
};

void printLevel(NodePtr_t end) {

  
  if(!(end.get())) return;
  NodePtr_t sp=end.get()->previous.lock();

  std::cout << end.get()->data << ((sp.get()) ? "->" : "->nil\n");
  printLevel(sp);
  return;
}

int main(int argc, char* argv[]) {

  size_t nodes=64;
  if(argc  > 1) {
    int temp = 0;
    temp = atoi(argv[1]);
    if(temp > 0) nodes = static_cast<size_t>(temp);
  }
  std::vector<NodePtr_t> myVector;
  myVector.reserve(nodes);
  Node_t currNode;
  for(size_t i=0; i < nodes; ++i) {
    NodePtr_t currNode = std::make_shared<Node_t>();
    currNode.get()->data=i+1;
    myVector.push_back(currNode);
  }

  for(size_t i=0; i < nodes; ++i) {
    size_t leftIndex  = i*2+1;
    size_t rightIndex = leftIndex+1;
    if(leftIndex  < nodes) myVector[i]->left   = myVector[leftIndex];
    if(rightIndex < nodes) myVector[i]->right  = myVector[rightIndex];
  }

  NodePtr_t root = myVector[0];
  updateTree(root);
  for(size_t i=0; i < nodes; ++i) {
    printLevel(myVector[i]);
  }
  return 0;
}
