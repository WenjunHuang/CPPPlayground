//
// Created by rick on 2020/2/22.
//
#include <iostream>
#include <filesystem>
#include <cstdlib>

int main(int argc,char*argv[]) {
  if (argc < 2){
      std::cout << "Usage: " << argv[0] <<" <path>\n";
      return EXIT_FAILURE;
  }

  namespace fs = std::filesystem;

  switch(fs::path p{argv[1]}; status(p).type()) {
  case fs::file_type::not_found:
      std::cout << "path \"" << p.string() << "\" does not exist\n";
      break;
  case fs::file_type::regular:
      std::cout << '"' << p.string() << "\n exists with " << file_size(p) << " bytes\n";
      break;
  case fs::file_type::directory:
      std::cout << '"' << p.string() << "\" is a directory" << '\n';
      for(const auto& e: fs::directory_iterator(p)) {
          std::cout << "  " << e.path().string() << '\n';
      }
      break;
  default:
      std::cout << '"' << p.string() << "\" is a special file\n";
      break;
  }

}