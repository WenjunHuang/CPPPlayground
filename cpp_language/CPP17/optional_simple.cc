#include <complex>
#include <optional>

int main() {
    // empty
    std::optional<int> oEmpty;
    std::optional<float> oFloat = std::nullopt;

    // direct:
    std::optional<int> oInt(10);
    std::optional oIntDeduced(10);

    // make_optional
    auto oDouble  = std::make_optional(3.0);
    auto oComplex = std::make_optional<std::complex<double>>(3.0, 4.0);

    // in_place
    std::optional<std::complex<double>> o7{std::in_place, 3.0, 4.0};
}