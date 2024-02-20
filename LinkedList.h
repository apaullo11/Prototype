class LinkedList {
  LinkedList() {};
  int size = 0;
  Node *head, *end;
  
  public:
  void removeNode(int index);
  void addNode(int index);
  void addFrontNode();
  void addBackNode();

};

//LinkedList::LinkedList(int s) {}

void addFrontNode() {
  this->head->next = new Node(NULL, this->head);
  this->head = this->head->next;
}

void addBackNode() {
  
}

struct Node {
  Node(Node *next, Node *prev);
  Node *next, *prev;
}

Node::Node(Node *next, Node *prev) {
  this->next = next;
  this->prev = prev;
}
