//
// Created by rick on 2017/7/24.
//
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/random_access_index.hpp>
#include <boost/multi_index/mem_fun.hpp>
#include <boost/multi_index/identity.hpp>
#include <utility>
#include <string>
#include <gtest/gtest.h>

using namespace boost::multi_index;
using namespace std;

class animal {
public:
  animal(string name, int legs) : _name{move(name)}, _legs{legs} {}

  bool operator<(const animal &a) const { return _legs < a._legs; }

  const string &name() const { return _name; }

private:
  string _name;
  int _legs;
};

using animal_multi = multi_index_container<
  animal,
  indexed_by<
    ordered_unique<
      identity<animal>
    >,
    hashed_unique<
      const_mem_fun<
        animal, const std::string &, &animal::name
      >
    >
  >
>;

TEST(MultiIndex, Key) {
  animal_multi animals;
  animals.emplace("cat", 4);
  animals.emplace("shark", 0);
  animals.emplace("spider", 8);

  cout << animals.begin()->name() << endl;
  const auto& name_index = animals.get<1>();
  cout << name_index.count("shark") << endl;
}
