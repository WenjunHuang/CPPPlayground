//
// Created by xxzyjy on 25/11/2017.
//
#include <iostream>
#include "catch.hpp"

struct Node{
  int value;
  Node *left;
  Node *right;
  Node(int i = 0):value(i),left(nullptr),right(nullptr){}
};

auto left = &Node::left;
auto right = &Node::right;

template<typename T,typename... TP>
Node* traverse(T np,TP... paths){
  return (np ->* ... ->* paths);
};

TEST_CASE("fold_traverse","[fold]"){
  Node *root = new Node{0};
  root->left = new Node{1};
  root->left->right = new Node{2};

  Node* node = traverse(root,left,right);
  REQUIRE(node->value == 2);

  node = traverse(root,left);
  REQUIRE(node->value == 1);
}

template<typename T>
class AddSpace {
private:
  const T& ref;
public:
  AddSpace(const T& value):ref(value){}
  friend std::ostream& operator<<(std::ostream& os,AddSpace<T> s){
    return os << s.ref << ' ';
  }
};
template<typename... Types>
void print(const Types &... args){
  (std::cout << ... << AddSpace<Types>(args)) << std::endl;
}

template<typename T1, typename...TN>
constexpr bool isHomogeneous(T1,TN...) {
  return (std::is_same<T1,TN>::value && ...);
};

TEST_CASE("is_home","[vard]"){
  REQUIRE(isHomogeneous(43,-1,"hello") == false);
}

TEST_CASE("fold_print","[fold]"){
  print(7.5,std::string("hello"),"world");

}

template<typename C,typename...Idx>
void printElems(const C& coll,Idx... idx){
  print(coll[idx]...);
};

TEST_CASE("printElems","[variadic]"){
  std::vector<std::string> coll = {"good","times","say","bye"};
  printElems(coll,2,0,3);
}

template<std::size_t...Idx,typename C>
void printIdx(const C& coll){
  print(coll[Idx]...);
};

TEST_CASE("printIdx","[variadic]"){
  std::vector<std::string> coll = {"good","times","say","bye"};
  printIdx<2,0,3>(coll);
}

