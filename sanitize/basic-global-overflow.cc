//
// Created by rick on 2022/2/9.
//
#include <stdio.h>
int x[100];

int main(){
  printf("Hello!\n");
  x[100] = 5; // Boom!
  return 0;
}