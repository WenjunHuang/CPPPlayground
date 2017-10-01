//
// Created by xxzyjy on 27/07/2017.
//
#include <boost/heap/priority_queue.hpp>
#include <iostream>
#include <gtest/gtest.h>

using namespace boost;
using namespace std;

TEST(Heap, Demo) {
  heap::priority_queue<int> pg;
  pg.push(2);
  pg.push(3);
  pg.push(1);

  for (int i : pg)
    cout << i << endl;

  heap::priority_queue<int> pg2;
  pg2.push(4);
  cout << boolalpha << (pg > pg2) << endl;
}

