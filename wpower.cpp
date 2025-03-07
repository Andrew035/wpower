#include "wpower.h"
#define SPACE 10 // for print 2D function for testing purposes

WirelessPower::WirelessPower(TREETYPE type) {
  m_type = type;
  m_root = nullptr;
}

WirelessPower::~WirelessPower() { clear(); }

void WirelessPower::clear() { clear(m_root); }

void WirelessPower::clear(Customer *customer) {
  if (customer != nullptr) {
    clear(customer->m_left);
    clear(customer->m_right);
    delete customer;
    customer = nullptr;
  }
}

void WirelessPower::insert(const Customer &customer) {
  switch (m_type) {
  case BST:
    m_root = insert(m_root, customer);
    break;
  case AVL:
    m_root = insert(m_root, customer);
    break;
  case SPLAY:
    m_root = insert(m_root, customer);
    break;
  }
}

Customer *&WirelessPower::insert(Customer *&root, const Customer &customer) {
  if (root == nullptr) {
    root = new Customer(customer); // creates a new node with customer
  }
  if (customer.getID() != root->getID()) {  // if we haven't found id yet
    if (customer.getID() < root->getID()) { // move left
      Customer *leftChild = root->getLeft();
      root->setLeft(insert(leftChild, customer));  // keep moving left
    } else if (customer.getID() > root->getID()) { // move right
      Customer *rightChild = root->getRight();
      root->setRight(insert(rightChild, customer)); // keep moving right
    }
    if (root != nullptr) {
      updateHeight(root); // update height
      if (m_type == AVL) {
        root = balance(root); // balance if avl type
      } else if (m_type == SPLAY) {
        root = splay(root, customer); // splay rotations if splay type
      }
    }
  }
  return root;
}

Customer *&WirelessPower::balance(Customer *&root) {
  if (root != nullptr && (getBalanceFactor(root) > 1 ||
                          getBalanceFactor(root) < -1)) { // if unbalanced
    if (getBalanceFactor(root) > 1) {
      Customer *left = root->getLeft();
      if (getBalanceFactor(left) >= 0) { // check if greater than or equal to 0
        return rotateRight(root);
      } else {
        root->setLeft(rotateLeft(left)); // set left of root to the rotate left
        return rotateRight(root);
      }
    } else if (getBalanceFactor(root) < -1) {
      Customer *right = root->getRight();
      if (getBalanceFactor(right) >= 0) { // check if greater than or equal to 0
        return rotateLeft(root);
      } else {
        root->setRight(
            rotateRight(right)); // set left of root to the rotate left
        return rotateLeft(root);
      }
    }
  }
  return root;
}

int WirelessPower::getBalanceFactor(const Customer *customer) const {
  if (customer == nullptr) {
    return 0;
  }
  int leftHeight = 0;
  int rightHeight = 0;

  if (customer->getLeft() != nullptr) // if we have a left child
  {
    leftHeight = (customer->getLeft()->getHeight()); // keep the height
  }
  if (customer->getRight() != nullptr) // if we have a right child
  {
    rightHeight = (customer->getRight()->getHeight()); // keep the height
  }
  return leftHeight - rightHeight; // get the factor of both
}

Customer *&WirelessPower::rotateRight(Customer *&customer) {
  if (customer != nullptr && customer->getLeft() != nullptr) {
    Customer *newRoot = customer->getLeft();
    Customer *right = newRoot->getRight();
    newRoot->setRight(customer); // move customer to the right

    customer->setLeft(right); // then move the new root left
    updateHeight(customer);   // update the customer node height
    updateHeight(newRoot);    // update the newRoot height
    customer = newRoot;       // set the customer to the newRoot
  }
  return customer;
}

Customer *&WirelessPower::rotateLeft(Customer *&customer) {
  // complete opposite of rotate right
  if (customer != nullptr && customer->getRight() != nullptr) {
    Customer *newRoot = customer->getRight();
    Customer *right = newRoot->getLeft();
    newRoot->setLeft(customer);

    customer->setRight(right);
    updateHeight(customer);
    updateHeight(newRoot);
    customer = newRoot;
  }
  return customer;
}

Customer *&WirelessPower::rotateLeftRight(Customer *&customer) {
  if (customer != nullptr && customer->getLeft() != nullptr) {
    Customer *left = customer->getLeft();
    customer->setLeft(rotateLeft(left)); // set left to the rotation of left
    customer = rotateRight(customer);    // set customer node to the rotation of
                                         // customer node to the right
  }
  return customer;
}

Customer *&WirelessPower::rotateRightLeft(Customer *&customer) {
  // opposite of zig zag
  if (customer != nullptr && customer->getRight() != nullptr) {
    Customer *right = customer->getRight();
    customer->setLeft(rotateRight(right));
    customer = rotateLeft(customer);
  }
  return customer;
}

Customer *&WirelessPower::splay(Customer *&root, const Customer &customer) {

  if (customer.getID() < root->getID()) // if id is less the roots id move left
  {
    if (root->getLeft() == nullptr) // once there are no left children
    {
      return root; // we return the root
    }
    if (customer.getID() <
        root->getLeft()
            ->getID()) // checks the left childs id and keeps moving left
    {
      Customer *leftLeft = root->getLeft()->getLeft();
      root->getLeft()->setLeft(splay(leftLeft, customer)); // zig zig
      root = rotateRight(root);
    } else if (customer.getID() > root->getLeft()->getID()) // go to right child
    {
      Customer *leftRight = root->getLeft()->getRight();
      root->getLeft()->setRight(splay(leftRight, customer)); // zig zag
      if (leftRight != nullptr) {
        Customer *leftChild = root->getLeft();
        root->setLeft(rotateLeft(leftChild));
      }
    }
    if (root != nullptr) {
      updateHeight(root);
    }

    return (root->getLeft() == nullptr) ? root : rotateRight(root);
  } else {
    if (root->getRight() == nullptr) {
      Customer *temp = m_root;
      m_root = new Customer(customer);
      m_root->setLeft(temp);
      return root;
    }
    if (customer.getID() > root->getRight()->getID()) // move right
    {
      Customer *rightRight = root->getRight()->getRight();
      root->getRight()->setRight(splay(rightRight, customer)); // zag zag
      root = rotateLeft(root);
    } else if (customer.getID() < root->getRight()->getID()) // move left
    {
      Customer *rightLeft = root->getRight()->getLeft();
      root->getRight()->setLeft(splay(rightLeft, customer)); // zag zig
      if (rightLeft != nullptr) {
        Customer *rightChild = root->getRight();
        root->setRight(rotateRight(rightChild));
      }
    }
    if (root != nullptr) {
      updateHeight(root);
    }

    return (root->getRight() == nullptr) ? root : rotateLeft(root);
  }
}

int WirelessPower::getHeight(Customer *customer) const {
  if (customer == nullptr) {
    return -1;
  }
  return customer->getHeight();
}

void WirelessPower::updateHeight(Customer *customer) {
  if (customer == nullptr) {
    return;
  }
  customer->setHeight(
      1 + max(getHeight(customer->getLeft()), getHeight(customer->getRight())));
}

void WirelessPower::remove(int id) {
  switch (m_type) {
  case BST:
    m_root = remove(m_root, id);
    break;
  case AVL:
    m_root = remove(m_root, id);
    m_root = balance(m_root);
    break;
  case SPLAY:
    break;
  }
}

Customer *&WirelessPower::remove(Customer *&root, int id) {
  if (root != nullptr) {
    if (id == root->getID()) {
      if (root->getLeft() == nullptr &&
          root->getRight() == nullptr) // delete root that we are currently at
      {
        delete root;
        root = nullptr;
      } else if (root->getLeft() == nullptr &&
                 root->getRight() != nullptr) // delete node and move right node
                                              // to sub root or root
      {
        Customer *oldRoot = root;
        root = root->getRight();
        delete oldRoot;
        oldRoot = nullptr;
      } else if (root->getLeft() != nullptr &&
                 root->getRight() == nullptr) // samething just left now
      {
        Customer *oldRoot = root;
        root = root->getLeft();
        delete oldRoot;
        oldRoot = nullptr;
      } else if (root->getLeft() != nullptr &&
                 root->getRight() != nullptr) // both have data
      {
        Customer *successor = root->getRight();
        if (successor != nullptr) // check right first
        {
          while (successor->getLeft() != nullptr) // check left
          {
            successor = successor->getLeft();
          }
        }
        Customer *oldRoot = root;
        root = new Customer(*successor);     // create new node
        root->setLeft(oldRoot->getLeft());   // set left node of newnode
        root->setRight(oldRoot->getRight()); // set right of newnode

        delete oldRoot;
        oldRoot = nullptr;

        Customer *rightChild = root->getRight();
        root->setRight(remove(rightChild, successor->getID()));
      }
    } else if (id < root->getID()) {
      Customer *leftChild = root->getLeft();
      root->setLeft(remove(leftChild, id));
    } else if (id > root->getID()) {
      Customer *rightChild = root->getRight();
      root->setRight(remove(rightChild, id));
    }

    if (root != nullptr) {
      updateHeight(root);
      if (m_type == AVL) {
        root = balance(root);
      }
    }
  }
  return root;
}

Customer *WirelessPower::findMin(Customer *customer) const {
  if (customer == nullptr) {
    return nullptr;
  }
  while (customer->getLeft() != nullptr) {
    customer = customer->getLeft();
  }
  return customer;
}

TREETYPE WirelessPower::getType() const { return m_type; }

bool WirelessPower::checkBalance() const {
  bool isBalanced = true;
  if (m_root != nullptr) {
    isBalanced = checkBalance(m_root);
  }
  return isBalanced;
}

Customer *&WirelessPower::restructureIntoAVL(Customer *&root) {
  if (root == nullptr) {
    return root;
  }
  Customer *leftChild = root->getLeft();
  Customer *rightChild = root->getRight();
  root->setLeft(restructureIntoAVL(leftChild));
  root->setRight(restructureIntoAVL(rightChild));

  updateHeight(root);
  root = balance(root);

  return root;
}

void WirelessPower::setType(TREETYPE type) {
  if (m_type != type) {
    m_type = type;
    if (m_type == AVL) {
      m_root = restructureIntoAVL(m_root);
    }
  }
}
bool WirelessPower::operator==(const WirelessPower &rhs) const {
  if ((m_root != nullptr && rhs.m_root == nullptr) ||
      (m_root == nullptr && rhs.m_root != nullptr)) {
    return false;
  } else if (m_root == nullptr && rhs.m_root == nullptr) {
    return true;
  }

  return equalityOperator(m_root, rhs.m_root);
}

bool WirelessPower::equalityOperator(const Customer *lhs,
                                     const Customer *rhs) const {
  if (lhs == nullptr && rhs == nullptr) {
    return true;
  } else if (lhs == nullptr || rhs == nullptr) {
    return false;
  } else {
    if (lhs->getID() != rhs->getID() || lhs->getHeight() != rhs->getHeight()) {
      return false;
    }
  }
  return (equalityOperator(lhs->getLeft(), rhs->getLeft()) &&
          equalityOperator(lhs->getRight(), rhs->getRight()));
}

const WirelessPower &WirelessPower::operator=(const WirelessPower &rhs) {
  if (!(*this == rhs)) {
    if (m_root != nullptr) {
      clear();
    }
    if (rhs.m_root == nullptr) {
      return *this;
    }
    Customer *rhsRoot = rhs.m_root;
    m_root = copyTree(rhsRoot);
  }
  return *this;
}

Customer *WirelessPower::copyTree(Customer *&root) {
  Customer *newNode = nullptr;
  if (root != nullptr) {
    newNode = new Customer(*root);

    Customer *leftChild = root->getLeft();
    Customer *rightChild = root->getRight();
    newNode->setLeft(copyTree(leftChild));
    newNode->setRight(copyTree(rightChild));

    return newNode;
  }
  return newNode;
}

void WirelessPower::dumpTree() const { dump(m_root); }

void WirelessPower::dump(Customer *customer) const {
  if (customer != nullptr) {
    cout << "(";
    dump(customer->m_left); // first visit the left child
    cout << customer->m_id << ":"
         << customer->m_height; // second visit the node itself
    dump(customer->m_right);    // third visit the right child
    cout << ")";
  }
}

ostream &operator<<(ostream &sout, const Customer &x) {
  sout << x.m_id << " (" << x.getLatStr() << ", " << x.getLongStr() << ")"
       << endl;
  ;
  return sout;
}

void WirelessPower::print2D(Customer *customer, int space) {
  if (customer == nullptr) {
    return;
  }
  space += SPACE;
  print2D(customer->getRight(), space);
  cout << endl;
  for (int i = SPACE; i < space; i++) {
    cout << " ";
  }
  cout << customer->getID() << ":" << customer->getHeight() << endl;
  print2D(customer->getLeft(), space);
}

Customer *WirelessPower::getRoot() { return m_root; }

bool WirelessPower::checkPreservance() const {
  bool isPreserved = true;
  if (m_root != nullptr) {
    isPreserved = checkPreservance(m_root);
  }
  return isPreserved;
}

bool WirelessPower::checkPreservance(const Customer *customer) const {
  if (customer == nullptr) {
    return true;
  }
  if (customer->getID() < MINID || customer->getID() > MAXID) {
    return false;
  }
  if ((customer->getLeft() != nullptr &&
       customer->getID() <= customer->getLeft()->getID()) ||
      (customer->getRight() != nullptr &&
       customer->getID() >= customer->getRight()->getID())) {
    return false;
  }
  return checkPreservance(customer->getLeft()) &&
         checkPreservance(customer->getRight());
}

bool WirelessPower::isEmpty() const { return m_root == nullptr; }

bool WirelessPower::find(int id) const {
  bool pass = false;
  if (m_root != nullptr && id >= MINID && id <= MAXID) {
    pass = find(id, m_root);
  }
  return pass;
}

bool WirelessPower::find(int id, const Customer *customer) const {
  bool pass = false;
  if (customer != nullptr) {
    if (id == customer->getID()) {
      pass = true;
    } else if (id < customer->getID()) {
      pass = find(id, customer->getLeft()); // traverse
    } else {
      pass = find(id, customer->getRight()); // traverse
    }
  }
  return pass;
}

bool WirelessPower::checkHeight(Customer *&root) const {
  bool pass = true;
  int correctHeight = 0;
  int leftHeight = 0;
  int rightHeight = 0;

  if (root->getLeft() == nullptr && root->getRight() == nullptr) {
    correctHeight = 0;
  } else {
    if (root->getLeft() != nullptr) {
      leftHeight = root->getLeft()->getHeight(); // gets height of left child
    }
    if (root->getRight() != nullptr) {
      rightHeight = root->getRight()->getHeight(); // gets height of right child
    }
    correctHeight =
        1 + (max(leftHeight, rightHeight)); // keeps the correct height
  }

  while (pass != false) {
    if (root->getHeight() != correctHeight) // if heights don't match
    {
      pass = false; // then false
    }

    if (root->getLeft() != nullptr) {
      Customer *leftChild = root->getLeft();
      pass = pass && checkHeight(leftChild);
    }
    if (root->getRight() != nullptr) {
      Customer *rightChild = root->getRight();
      pass = pass && checkHeight(rightChild);
    }
    break; // gets out of while loop if never false which we want
  }

  return pass;
}
