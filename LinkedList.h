#include <cstdint>

class LinkedList {
  LinkedList();
  LinkedList(unsigned int size);
  unsigned int size = 0;
  Node *head, *tail;
  
  public:
  void removeNode(unsigned int index);
  void addNode(unsigned int index);
  void addFrontNode();
  void addBackNode();
  void removeFrontNode();
  void removeBackNode();
  void destroyList(uint8_t dir);
};

struct Node {
  Node();
  Node(Node *next, Node *prev);
  Node *next, *prev;
};

LinkedList::LinkedList() {
  this->head = nullptr;
  this->tail = nullptr;
}

LinkedList::LinkedList(unsigned int size) {
  Node *tempTail = nullptr; 
  for (unsigned int i = 0; i<size; i++) {
    if (tempTail == nullptr) {
      // set head on creating first node
      this->head = new Node(nullptr, nullptr);
      tempTail = this->head;
    } else {
      tempTail->prev = new Node(tempTail, nullptr);
      tempTail = tempTail->prev;
    }
  }
  this->tail = tempTail;
}

/* TODO
void LinkedList::addNode(unsigned int index) {
  if (index == 0) {
    addFrontNode();
  } else if (index == this->size) {
    addBackNode();
  } else if (index > this->size) {
    for (unsigned int i = 0; i<index; i++) {

    }
  } else {
    Node *next;
    unsigned int listIndex = (index > this->size/2) ? (this->size - index) : index;
    for (int i = 0; i<listIndex; i++) {
      if (index > this->size/2)  {
        
      } else {

      }
    }
  }
}
*/

void LinkedList::addFrontNode() {
  this->head->next = new Node(nullptr, this->head);
  this->head = this->head->next;
}

void LinkedList::addBackNode() {
  this->tail->prev = new Node(this->tail, nullptr);
  this->tail = this->tail->prev;
}

void LinkedList::removeFrontNode() {
  Node *oldHead = this->head;
  // Set new head
  this->head = oldHead->prev;
  // Remove next ptr on new head
  this->head->next = nullptr;  
  // Remove old head
  delete oldHead;
}

void LinkedList::removeBackNode() {
  Node *oldTail = this->tail;
  // Set new tail
  this->tail = oldTail->next;
  // Remove prev ptr on new head
  this->tail->prev = nullptr;  
  // Remove old head
  delete oldTail;
}

void LinkedList::destroyList(uint8_t dir) {
  Node *n;
  if (dir<0) {
    n = this->tail;
    while (n != nullptr) {
      if (n->next == nullptr) {
        delete n;
        n = nullptr;
      } else {
        n = n->next;
        delete n->prev;
      }
    }
  } else {
    n = this->head;
    while (n != nullptr) {
      if (n->prev == nullptr) {
        delete n;
        n == nullptr;
      } else {
        n = n->prev;
        delete n->next;
      }
    }
  }
}

Node::Node() {
  this->next = nullptr;
  this->prev = nullptr;
}

Node::Node(Node *next, Node *prev) {
  this->next = next;
  this->prev = prev;
}
