#pragma once

#include <utility>

template<typename Container,typename Index>
decltype(auto) authAndAccess(Container&& c,Index i){
//  authenticateUser();
  return std::forward<Container>(c)[i];
};