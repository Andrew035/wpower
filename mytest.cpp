#include "wpower.h"
#include <algorithm>
#include <math.h>
#include <random>
#include <vector>

enum RANDOM { UNIFORMINT, UNIFORMREAL, NORMAL, SHUFFLE };
class Random {
public:
  Random(int min, int max, RANDOM type = UNIFORMINT, int mean = 50,
         int stdev = 20)
      : m_min(min), m_max(max), m_type(type) {
    if (type == NORMAL) {
      // the case of NORMAL to generate integer numbers with normal distribution
      m_generator = std::mt19937(m_device());
      // the data set will have the mean of 50 (default) and standard deviation
      // of 20 (default) the mean and standard deviation can change by passing
      // new values to constructor
      m_normdist = std::normal_distribution<>(mean, stdev);
    } else if (type == UNIFORMINT) {
      // the case of UNIFORMINT to generate integer numbers
      //  Using a fixed seed value generates always the same sequence
      //  of pseudorandom numbers, e.g. reproducing scientific experiments
      //  here it helps us with testing since the same sequence repeats
      m_generator = std::mt19937(10); // 10 is the fixed seed value
      m_unidist = std::uniform_int_distribution<>(min, max);
    } else if (type == UNIFORMREAL) { // the case of UNIFORMREAL to generate
                                      // real numbers
      m_generator = std::mt19937(10); // 10 is the fixed seed value
      m_uniReal =
          std::uniform_real_distribution<double>((double)min, (double)max);
    } else { // the case of SHUFFLE to generate every number only once
      m_generator = std::mt19937(m_device());
    }
  }
  void setSeed(int seedNum) {
    // we have set a default value for seed in constructor
    // we can change the seed by calling this function after constructor call
    // this gives us more randomness
    m_generator = std::mt19937(seedNum);
  }
  void getShuffle(vector<int> &array) {
    // the user program creates the vector param and passes here
    // here we populate the vector using m_min and m_max
    for (int i = m_min; i <= m_max; i++) {
      array.push_back(i);
    }
    shuffle(array.begin(), array.end(), m_generator);
  }

  void getShuffle(int array[]) {
    // the param array must be of the size (m_max-m_min+1)
    // the user program creates the array and pass it here
    vector<int> temp;
    for (int i = m_min; i <= m_max; i++) {
      temp.push_back(i);
    }
    std::shuffle(temp.begin(), temp.end(), m_generator);
    vector<int>::iterator it;
    int i = 0;
    for (it = temp.begin(); it != temp.end(); it++) {
      array[i] = *it;
      i++;
    }
  }

  int getRandNum() {
    // this function returns integer numbers
    // the object must have been initialized to generate integers
    int result = 0;
    if (m_type == NORMAL) {
      // returns a random number in a set with normal distribution
      // we limit random numbers by the min and max values
      result = m_min - 1;
      while (result < m_min || result > m_max)
        result = m_normdist(m_generator);
    } else if (m_type == UNIFORMINT) {
      // this will generate a random number between min and max values
      result = m_unidist(m_generator);
    }
    return result;
  }

  double getRealRandNum() {
    // this function returns real numbers
    // the object must have been initialized to generate real numbers
    double result = m_uniReal(m_generator);
    // a trick to return numbers only with two deciaml points
    // for example if result is 15.0378, function returns 15.03
    // to round up we can use ceil function instead of floor
    result = std::floor(result * 100.0) / 100.0;
    return result;
  }

private:
  int m_min;
  int m_max;
  RANDOM m_type;
  std::random_device m_device;
  std::mt19937 m_generator;
  std::normal_distribution<> m_normdist;     // normal distribution
  std::uniform_int_distribution<> m_unidist; // integer uniform distribution
  std::uniform_real_distribution<double> m_uniReal; // real uniform distribution
};

class Tester {
private:
  Random idGen;
  Random latGen;
  Random longGen;

public:
  Tester()
      : idGen(MINID, MAXID), latGen(MINLAT, MAXLAT), longGen(MINLONG, MAXLONG) {
  }
  bool testAVLTreeBalance() {
    WirelessPower wp(AVL);
    int size = 10;
    for (int i = 0; i < size; i++) {
      int id = idGen.getRandNum();
      double lat = latGen.getRandNum();
      double lon = longGen.getRandNum();
      Customer customer(id, lat, lon);
      wp.insert(customer);
    }
    return wp.checkBalance();
  }
  bool testBSTProperty() {
    WirelessPower wp(BST);
    int size = 300;
    for (int i = 0; i < size; i++) {
      int id = idGen.getRandNum();
      double lat = latGen.getRandNum();
      double lon = longGen.getRandNum();
      Customer customer(id, lat, lon);
      wp.insert(customer);
    }
    return wp.checkPreservance();
  }
  bool testRemoveBSTNormal() {
    WirelessPower wp(BST);
    int size = 6;
    int ids[size + 1];
    for (int i = 0; i < size; i++) {
      int id = idGen.getRandNum();
      double lat = latGen.getRandNum();
      double lon = longGen.getRandNum();
      ids[i] = id;
      Customer customer(id, lat, lon);
      wp.insert(customer);
    }
    int remove = 3;
    wp.remove(ids[remove]);
    bool pass = false;
    if (!wp.find(ids[remove])) {
      pass = true;
      return pass;
    }
    return pass;
  }
  bool testRemoveBSTEdge() {
    WirelessPower wp(BST);
    int tempID = idGen.getRandNum();
    double lat = latGen.getRandNum();
    double lon = longGen.getRandNum();
    Customer customer(tempID, lat, lon);
    wp.insert(customer);
    wp.remove(tempID);

    return wp.isEmpty();
  }
  bool testSplay() {
    WirelessPower wp(SPLAY);
    int size = 10;
    bool pass = true;

    for (int i = 0; i < size; i++) {
      int id = idGen.getRandNum();
      double lat = latGen.getRandNum();
      double lon = longGen.getRandNum();

      Customer customer(id, lat, lon);
      wp.insert(customer);

      Customer *root = wp.getRoot();
      pass = pass && (root != nullptr && root->getID() == id);
      pass = pass && (wp.checkPreservance());
    }
    return pass;
  }
  bool testHeightAfterSplay() {
    WirelessPower wp(SPLAY);
    int size = 10;
    for (int i = 0; i < size; i++) {
      int id = idGen.getRandNum();
      double lat = latGen.getRandNum();
      double lon = longGen.getRandNum();

      Customer customer(id, lat, lon);
      wp.insert(customer);
    }
    Customer *root = wp.getRoot();
    return (wp.checkHeight(root));
  }
  bool testAVLBalancedAfterRemove() {
    WirelessPower wp(AVL);
    int size = 300;
    int numRemove = 134;
    int ids[size + 1];

    for (int i = 0; i < size; i++) {
      int id = idGen.getRandNum();
      double lat = latGen.getRandNum();
      double lon = longGen.getRandNum();

      Customer customer(id, lat, lon);
      wp.insert(customer);
      ids[i] = id;
    }

    for (int i = 0; i < size - numRemove; i++) {
      wp.remove(ids[i]);
    }

    return wp.checkBalance();
  }
  bool testBSTPropertyRemove() {
    WirelessPower wp(BST);
    int size = 300;
    int numRemove = 156;
    int ids[size + 1];

    for (int i = 0; i < size; i++) {
      int id = idGen.getRandNum();
      double lat = latGen.getRandNum();
      double lon = longGen.getRandNum();

      Customer customer(id, lat, lon);
      wp.insert(customer);
      ids[i] = id;
    }

    for (int i = 0; i < size - numRemove; i++) {
      wp.remove(ids[i]);
    }

    return wp.checkPreservance();
  }
  bool testBSTHeightRemove() {
    WirelessPower wp(BST);
    int size = 10;
    int numRemove = 2;
    int ids[size + 1];

    for (int i = 0; i < size; i++) {
      int id = idGen.getRandNum();
      double lat = latGen.getRandNum();
      double lon = longGen.getRandNum();

      Customer customer(id, lat, lon);
      wp.insert(customer);
      ids[i] = id;
    }

    for (int i = 0; i < size - numRemove; i++) {
      wp.remove(ids[i]);
    }
    Customer *root = wp.getRoot();

    return wp.checkHeight(root);
  }
  bool testAssignmentNormal() {
    WirelessPower wp(BST);
    WirelessPower ewp(BST);

    int size = 15;

    for (int i = 0; i < size; i++) {
      int id = idGen.getRandNum();
      double lat = latGen.getRandNum();
      double lon = longGen.getRandNum();

      Customer customer(id, lat, lon);
      wp.insert(customer);
    }
    ewp = wp;
    return (ewp == wp);
  }
  bool testAssignmentError() {
    WirelessPower ewp1(BST);
    WirelessPower ewp2(BST);

    ewp1 = ewp2;
    return (ewp1 == ewp2);
  }
};

int main() {
  Tester t;

  if (t.testAVLTreeBalance()) {
    cout << "Passed AVLTreeBalance" << endl;
  } else {
    cout << "Failed AVLTreeBalance" << endl;
  }
  if (t.testBSTProperty()) {
    cout << "Passed BSTProperty" << endl;
  } else {
    cout << "Failed BSTProperty" << endl;
  }
  if (t.testRemoveBSTNormal()) {
    cout << "Passed RemoveBSTNormal" << endl;
  } else {
    cout << "Failed RemoveBSTNormal" << endl;
  }
  if (t.testRemoveBSTEdge()) {
    cout << "Passed RemoveBSTEdge" << endl;
  } else {
    cout << "Failed RemoveBSTEdge" << endl;
  }
  if (t.testSplay()) {
    cout << "Passed Splay" << endl;
  } else {
    cout << "Failed Splay" << endl;
  }
  if (t.testHeightAfterSplay()) {
    cout << "Passed HeightAfterSplay" << endl;
  } else {
    cout << "Failed HeightAfterSplay" << endl;
  }
  if (t.testAVLBalancedAfterRemove()) {
    cout << "Passed AVLBalancedAfterRemove" << endl;
  } else {
    cout << "Failed AVLBalancedAfterRemove" << endl;
  }
  if (t.testBSTPropertyRemove()) {
    cout << "Passed BSTPropertyRemove" << endl;
  } else {
    cout << "Failed BSTPropertyRemove" << endl;
  }
  if (t.testBSTHeightRemove()) {
    cout << "Passed BSTHeightRemove" << endl;
  } else {
    cout << "Failed BSTHeightRemove" << endl;
  }
  if (t.testAssignmentNormal()) {
    cout << "Passed AssignmentNormal" << endl;
  } else {
    cout << "Failed AssignmentNormal" << endl;
  }
  if (t.testAssignmentError()) {
    cout << "Passed AssignmentError" << endl;
  } else {
    cout << "Failed AssignmentError" << endl;
  }
  return 0;
}
