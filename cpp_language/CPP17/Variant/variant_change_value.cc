#include <variant>
#include <string>
#include <complex>

int main() {
  std::variant<std::complex<int>, std::string> var{std::in_place_index<0>, 10, 10};
  var = "helloworld";
  var.emplace<0>(20,20);

  std::get<1>(var) = "wtf";
}