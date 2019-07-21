//
// Created by rick on 18-8-26.
//

#ifndef CLANGPLAYGROUND_HELPER_H
#define CLANGPLAYGROUND_HELPER_H
#include <vector>

#define VECTOR_STRING(name) \
    std::vector<std::string> name; \
    const char* get##name(unsigned i); \
    void add##name(const char* s); \
    unsigned get##name##Count(); \
    void clear##name();

#define VECTOR(type, name) \
  std::vector<type> name; \
  type get##name(unsigned i); \
  void add##name(type& s); \
  unsigned get##name##Count(); \
  void clear##name();

#define DEF_VECTOR(klass,type,name) \
  type klass::get##name(unsigned i){ return name[i];} \
  void klass::add##name(type& s) {return name.push_back(s);} \
  unsigned klass::get##name##Count() {return name.size();} \
  void klass::clear##name(){name.clear();}

#define DEF_VECTOR_STRING(klass,name) \
  const char* klass::get##name(unsigned i){return name[i].c_str();} \
  void klass::add##name(const char* s){return name.push_back(std::string(s));} \
  unsigned klass::get##name##Count(){ return name.size(); } \
  void klass::clear##name(){ name.clear(); }


#endif //CLANGPLAYGROUND_HELPER_H
