//
// Created by rick on 2017/7/24.
//

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/random_access_index.hpp>
#include <string>
#include <gtest/gtest.h>

using namespace std;
using namespace boost;
using namespace boost::multi_index;

struct animal {
  string name;
  int legs;
};

using animal_multi = multi_index_container<
  animal,
  indexed_by<hashed_non_unique<member<animal, string, &animal::name>>,
    hashed_non_unique<member<animal, int, &animal::legs>>
  >
>;

TEST(MultiIndex, NonUnique) {

  animal_multi animals;
  animals.insert({"cat", 4});
  animals.insert({"shark", 0});
  animals.insert({"spider", 8});

  cout << animals.count("cat") << endl;
  const animal_multi ::nth_index<1>::type &legs_index = animals.get<1>();
  cout << legs_index.count(8) << endl;

}

TEST(MultiIndex, Modify) {
 animal_multi animals;
  animals.insert({"cat", 4});
  animals.insert({"shark", 0});
  animals.insert({"spider", 8});

  auto &legs_index = animals.get<1>();
  auto it = legs_index.find(4);
  legs_index.modify(it, [](animal &a){a.name="dog";});
  cout << animals.count("dog") << endl;
}

TEST(MultiIndex, HashedUnique) {
  typedef multi_index_container<
    animal,
    indexed_by<
      hashed_non_unique<
        member<
          animal, std::string, &animal::name
        >
      >,
      hashed_unique<
        member<
          animal, int, &animal::legs
        >
      >
    >
  > animal_multi;
  animal_multi animals;
  animals.insert({"cat", 4});
  animals.insert({"shark", 0});
  animals.insert({"dog", 4});

  auto &legs_index = animals.get<1>();
  cout << legs_index.count(4) << endl;
}

TEST(MultiIndex, SeqOrderedNonUniqueRandom) {
  using animal_multi = multi_index_container<
    animal,
    indexed_by<
      sequenced<>,
      ordered_non_unique<
        member<
          animal, int, &animal::legs
        >
      >,
      random_access<>
      >
    >;
  animal_multi animals;

  animals.push_back({"cat", 4});
  animals.push_back({"shark", 0});
  animals.push_back({"spider", 8});

  auto &legs_index = animals.get<1>();
  auto it = legs_index.lower_bound(4);
  auto end = legs_index.upper_bound(8);
  ostream_iterator<string> output{cout, "\n"};
  std::for_each(it, end, [&output](auto i){
   *output++ = i.name;
  });

  const auto & rand_index = animals.get<2>();
  cout << rand_index[0].name << endl;
}