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
      m_generator = std::mt19937(m_device());
      m_normdist = std::normal_distribution<>(mean, stdev);
    } else if (type == UNIFORMINT) {
      m_generator = std::mt19937(10);
      m_unidist = std::uniform_int_distribution<>(min, max);
    } else if (type == UNIFORMREAL) {
      m_generator = std::mt19937(10);
      m_uniReal =
          std::uniform_real_distribution<double>((double)min, (double)max);
    } else {
      m_generator = std::mt19937(m_device());
    }
  }
  void setSeed(int seedNum) { m_generator = std::mt19937(seedNum); }

  void getShuffle(std::vector<int> &array) {
    for (int i = m_min; i <= m_max; i++) {
      array.push_back(i);
    }
    shuffle(array.begin(), array.end(), m_generator);
  }

  void getShuffle(int array[]) {
    std::vector<int> temp;
    for (int i = m_min; i <= m_max; i++) {
      temp.push_back(i);
    }
    std::shuffle(temp.begin(), temp.end(), m_generator);
    std::vector<int>::iterator it;
    int i = 0;
    for (it = temp.begin(); it != temp.end(); it++) {
      array[i] = *it;
      i++;
    }
  }

  int getRandNum() {
    int result = 0;
    if (m_type == NORMAL) {
      result = m_min - 1;
      while (result < m_min || result > m_max) {
        result = m_normdist(m_generator);
      }
    } else if (m_type == UNIFORMINT) {
      result = m_unidist(m_generator);
    }
    return result;
  }

  double getRealRandNum() {
    double result = m_uniReal(m_generator);
    result = std::floor(result * 100.0) / 100.0;
    return result;
  }

private:
  int m_min;
  int m_max;
  RANDOM m_type;
  std::random_device m_device;
  std::mt19937 m_generator;
  std::normal_distribution<> m_normdist;
  std::uniform_int_distribution<> m_unidist;
  std::uniform_real_distribution<double> m_uniReal;
};

class Tester {};

int main() {
  Random idGen(MINID, MAXID);
  Random rndLat(MINLAT, MAXLAT, UNIFORMREAL);
  rndLat.setSeed(5);
  Random rndLong(MINLONG, MAXLONG, UNIFORMREAL);
  rndLong.setSeed(9);
  {
    WirelessPower powerSrc(BST);
    int custSize = 10;
    int tempID = 0;
    int ID = 0;
    cout << "Inserting nodes: " << endl;
    for (int i = 0; i < custSize; i++) {
      ID = idGen.getRandNum();
      if (i == custSize / 2)
        tempID = ID;
      Customer customer(ID, rndLat.getRealRandNum(), rndLong.getRealNum());
      powerSrc.insert(customer);
      cout << customer;
    }
    cout << "\nDump after inserting " << custSize << " nodes into BST tree:\n";
    cout << endl;
    powerSrc.dumpTree();
    cout << endl;
    powerSrc.setType(AVL);
    cout << "\nDump after converting tree from BST to AVL:\n";
    cout << endl;
    powerSrc.dumpTree();
    cout << endl;
    powerSrc.remove(tempID);
    cout << "\nDump after removing the node ID " << tempID
         << " from the AVL tree:\n";
    cout << endl;
    powerSrc.dumpTree();
    cout << endl;
  }
  return 0;
}
