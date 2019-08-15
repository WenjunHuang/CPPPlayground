//
// Created by rick on 2017/7/25.
//

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/
#include <iostream>
#include "gtest/gtest.h"

using namespace std;
using namespace boost;
using namespace boost::property_tree;

TEST(PropertyTree, AccessingData) {
  ptree pt;
  pt.put("C:.Windows.System", "20 files");

  ptree &c = pt.get_child("C:");
  ptree &windows = c.get_child("Windows");
  ptree &system = windows.get_child("System");
  cout << system.get_value<string>() << endl;
}

TEST(PropertyTree, AccessingData2) {
  using ptree = basic_ptree<string, int>;
  ptree pt;
  pt.put(ptree::path_type{"C:\\Windows\\System", '\\'}, 20);
  pt.put(ptree::path_type{"C:\\Windows\\Cursors", '\\'}, 50);
  ptree &windows = pt.get_child(ptree::path_type{"C:\\Windows", '\\'});
  int files = 0;
  for (const auto &p : windows)
    files += p.second.get_value<int>();
  cout << files << endl;
}

struct string_to_int_translator {
  using internal_type = string;
  using external_type = int;

  optional<int> get_value(const string &s) {
    char *c;
    long l = strtol(s.c_str(), &c, 10);
    return make_optional(c != s.c_str(), static_cast<int>(l));
  }
};

TEST(PropertyTree, accessing_data_with_a_translator) {
  using ptree = property_tree::iptree;
  ptree pt;
  pt.put(ptree::path_type{"C:\\Windows\\System", '\\'}, "20 files");
  pt.put(ptree::path_type{"C:\\Windows\\Cursors", '\\'}, "50 files");

  string_to_int_translator tr;
  int files = pt.get<int>(ptree::path_type{"c:\\windows\\system", '\\'}, tr) +
              pt.get<int>(ptree::path_type{"c:\\windows\\cursors", '\\'}, tr);
  cout << files << endl;
}

TEST(PropertyTree, various_member_functions) {
  using boost::property_tree::ptree;
  ptree pt;
  pt.put("C:.Windows.System", "20 files");

  optional<string> c = pt.get_optional<string>("C:");
  cout << boolalpha << c.is_initialized() << endl;

  pt.put_child("D:.Program Files", ptree{"50 files"});
  pt.add_child("D:.Program Files", ptree{"60 files"});

  ptree d = pt.get_child("D:");
  for (const auto &p:d)
    cout << p.second.get_value<string>() << endl;

  optional<ptree&> e = pt.get_child_optional("E:");
  cout << e.is_initialized() << endl;
}

TEST(PropertyType, serializing) {
  using namespace boost::property_tree;
  ptree pt;
  pt.put("C:.Windows.System","20 files");
  pt.put("C:.Windows.Cursors","50 files");

  json_parser::write_json("file.json", pt);

  ptree pt2;
  json_parser::read_json("file.json",pt2);

  cout << boolalpha << (pt == pt2) << endl;

}