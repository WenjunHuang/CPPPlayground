//
// Created by rick on 2017/7/24.
//

#include <boost/bimap.hpp>
#include <boost/bimap/multiset_of.hpp>
#include <string>
#include <iostream>
#include <string>
#include <gtest/gtest.h>

using namespace std;
using namespace boost;

TEST(Bimap, Animal) {
  using bimap=bimap<string, int>;
  bimap animals;

  animals.insert({"cat", 4});
  animals.insert({"shark", 0});
  animals.insert({"spider", 8});

  cout << animals.left.count("cat") << endl;
  cout << animals.right.count(8) << endl;

  for (auto it = animals.begin(); it != animals.end(); ++it)
    cout << it->left << " has " << it->right << " legs" << endl;
}

TEST(Bimap, SetOf) {
  typedef boost::bimap<boost::bimaps::set_of<std::string>,
    boost::bimaps::set_of<int>> bimap;
  bimap animals;

  animals.insert({"cat", 4});
  animals.insert({"shark", 0});
  animals.insert({"spider", 8});

  cout << animals.left.count("spider") << endl;
  cout << animals.right.count(8) << endl;
}

TEST(Bimap, MultiSetOf) {
  typedef boost::bimap<boost::bimaps::set_of<std::string>,
    boost::bimaps::multiset_of<int>> bimap;
  bimap animals;

  animals.insert({"cat", 4});
  animals.insert({"shark", 0});
  animals.insert({"dog", 4});

  cout << animals.left.count("dog") << endl;
  cout << animals.right.count(4) << endl;
}