#include <stdint.h>

//---- VEC2 DECLARATIONS ----//
struct vec2 {
  vec2();
  vec2(int16_t x, int16_t y); //, int16_t color);
  //~vec2();
  int16_t x, y;
  void add(vec2 v);
  void add(vec2 *v);
  inline vec2 operator+(vec2 v);
  inline vec2 operator-(vec2 v);
  inline vec2 operator*(int n);
  inline vec2 operator/(int n);
  inline bool operator==(vec2 v);
};

vec2::vec2() { //, int16_t color) {
  this->x = 0;
  this->y = 0;
  //this->color = color;
}

vec2::vec2(int16_t x, int16_t y) { //, int16_t color) {
  this->x = x;
  this->y = y;
  //this->color = color;
}

vec2 vec2::operator+(vec2 v) {
  vec2 newV;
  newV.x = this->x + v.x;
  newV.y = this->y + v.y;
  return newV;
}

vec2 vec2::operator-(vec2 v) {
  vec2 newV;
  newV.x = this->x - v.x;
  newV.y = this->y - v.y;
  return newV;
}

vec2 vec2::operator*(int n) {
  vec2 newV;
  newV.x = this->x * n;
  newV.y = this->y * n;
  return newV;
}

vec2 vec2::operator/(int n) {
  vec2 newV;
  newV.x = this->x / n;
  newV.y = this->y / n;
  return newV;
}

bool vec2::operator==(vec2 v) {
  return ( (this->x == v.x) && (this->y == v.y) );
}

void vec2::add(vec2 v) {
  this->x = this->x + v.x;
  this->y = this->y + v.y;
}

void vec2::add(vec2 *v) {
  this->x = this->x + v->x;
  this->y = this->y + v->y;
}
//---- END OF VEC2 DECLARATIONS ----//

vec2 add(vec2 v1, vec2 v2) {
  return vec2(v1.x+v2.x, v1.y+v2.y);
}
/*
vec2 abs(vec2 v) {
  return vec2(abs(v.x), abs(v.y));
}
*/
//---- NODE DECLARATIONS ----//
struct Node {
  Node();
  Node(Node *next, Node *prev);
  ~Node();
  Node *next, *prev;
  vec2 *pos;
  vec2 *dir;
};

Node::Node() {
  this->next = nullptr;
  this->prev = nullptr;
  this->pos = new vec2();
  this->dir = new vec2();
}

Node::Node(Node *next, Node *prev) {
  this->next = next;
  this->prev = prev;
  this->pos = new vec2();
  this->dir = new vec2();
}

Node::~Node() {
  delete pos;
  delete dir;
}
//---- END OF NODE DECLARATIONS ----//

//---- LINKEDLIST DECLARATIONS ----//
struct LinkedList {
  unsigned int size = 0;
  Node *head, *tail;
  
  LinkedList();
  LinkedList(unsigned int size);
  ~LinkedList();
  void removeNode(unsigned int index);
  void addNode(unsigned int index);
  Node* addHeadNode();
  Node* addHeadNode(vec2 pos, vec2 dir);
  Node* addTailNode();
  Node* addTailNode(vec2 pos, vec2 dir);
  Node* removeHeadNode();
  void destroyHeadNode();
  Node* removeTailNode();
  void destroyTailNode();
  void moveBackToFront();
  void moveBackToFront(vec2 *newDir);
  void destroyList();
  void destroyList(uint8_t dir);
  Node* pop();
  void push(Node* nodeptr);
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
  this->size = size;
}

LinkedList::~LinkedList() {

}

/* TODO
void LinkedList::addNode(unsigned int index) {
  if (index == 0) {
    addHeadNode();
  } else if (index == this->size) {
    addTailNode();
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

Node* LinkedList::addHeadNode() {
  if (this->size == 0) {
    this->head = new Node(nullptr, nullptr);
    this->tail = this->head;
  } else {
    this->head->next = new Node(nullptr, this->head);
    this->head = this->head->next;
  }
  this->size++;
  return this->head;
}

Node* LinkedList::addHeadNode(vec2 pos, vec2 dir) {
  addHeadNode();
  *(this->head->pos) = pos;
  *(this->head->dir) = dir;
  return this->head;
}

Node* LinkedList::addTailNode() {
  if (this->size == 0) {
    this->tail = new Node(nullptr, nullptr);
    this->head = this->tail;
  } else {
    this->tail->prev = new Node(this->tail, nullptr);
    this->tail = this->tail->prev;
  }
  this->size++;
  return this->tail;
}

Node* LinkedList::addTailNode(vec2 pos, vec2 dir) {
  addTailNode();
  *(this->tail->pos) = pos;
  *(this->tail->dir) = dir;
  return this->tail;
}

Node* LinkedList::removeHeadNode() {
  Node *oldHead = this->head;
  // Set new head
  this->head = oldHead->prev;
  // Remove next ptr on new head
  this->head->next = nullptr;  
  oldHead->prev = nullptr;
  this->size--;

  return oldHead;
}

void LinkedList::destroyHeadNode() {
  Node *oldHead = removeHeadNode();
  // Delete old head
  delete oldHead;
}

Node* LinkedList::removeTailNode() {
  Node *oldTail = this->tail;
  // Set new tail
  this->tail = oldTail->next;
  // Remove prev ptr on new head
  this->tail->prev = nullptr;
  oldTail->next = nullptr;
  this->size--;

  return oldTail;
}

void LinkedList::destroyTailNode() {
  Node *oldTail = removeTailNode();
  // Delete old head
  delete oldTail;
}

void LinkedList::destroyList() {
  destroyList(-1);
}

void LinkedList::destroyList(uint8_t dir) {
  Node *n;
  if (dir<0) {
    n = this->tail;
    while (n != nullptr) {
      if (n->next == nullptr) {
        delete n->dir;
        delete n->pos;
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
        delete n->dir;
        delete n->pos;
        delete n;
        n == nullptr;
      } else {
        n = n->prev;
        delete n->next;
      }
    }
  }
}

Node* LinkedList::pop() {
  // TODO
}

void LinkedList::push(Node* nodeptr) {
  this->head->next = nodeptr;
  nodeptr->prev = head;
  this->head = nodeptr;
  this->size++;
}

// maybe make a pop and push function for these?
// Move Node to front with same direction
void LinkedList::moveBackToFront() {
  Node *oldTail = removeTailNode();

  oldTail->pos = this->head->pos;
  oldTail->pos->add(this->head->dir);
  oldTail->dir = this->head->dir;

  this->push(oldTail);
}
// Move node to front with dif direction
void LinkedList::moveBackToFront(vec2 *newDir) {
  Node *oldTail = removeTailNode();

  oldTail->pos->add(newDir);
  oldTail->dir = newDir;
  
  this->push(oldTail);
}