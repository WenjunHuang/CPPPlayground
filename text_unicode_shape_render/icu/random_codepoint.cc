//
// Created by HUANG WEN JUN on 2021/9/13.
//
#include <cstdlib>
#include <cstdio>
// to determine codepoint categories
#include <unicode/uchar.h>
// to output UTF-32 codepoints in proper encoding for terminal
#include <unicode/ustdio.h>

int main() {
  auto f = fopen("/dev/urandom","rb");
  UFILE *out;

  // UTF-32 code unit can hold an entire codepoint
  UChar32 c;


  if (!f){
    fputs("Unable to open /dev/urandom\n",stderr);
    return EXIT_FAILURE;
  }

  out = u_get_stdout();
  while(fread(&c,sizeof c,1,f)) {
    c %= 0xF0000;
    auto cat = u_charType(c);
    if (cat != U_UNASSIGNED && cat != U_PRIVATE_USE_CHAR && cat != U_SURROGATE){
      u_fputc(c,out);
    }
  }

  fclose(f);
}