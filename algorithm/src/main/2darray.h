//
// Created by xxzyjy on 30/09/2017.
//

#ifndef CPPALGORITHM_2DARRAY_H
#define CPPALGORITHM_2DARRAY_H

#include <cstdint>
#include <memory>

namespace alg {
  template<typename T=char>
  class Array2D {
  private:
    uint32_t NR;
    uint32_t NC;
    std::unique_ptr<T[]> _data;
  public:
    Array2D(uint32_t nrow, uint32_t ncol) : NR{nrow},
                                            NC{ncol},
                                            _data{std::make_unique<T[]>(nrow * ncol)} {}

    Array2D(const Array2D &) = delete;

    Array2D &operator=(const Array2D &) = delete;

    uint32_t row() const { return NR; }

    uint32_t col() const { return NC; }

    T &operator()(int row, int col) {
      return _data[row * NR + col];
    }

    T *operator[](int row) {
      return &_data[row * NR];
    }

    const T *operator[](int row) const {
      return &_data[row * NR];
    }

    void clear(const T &value) {
      for (auto i = 0; i < NR * NC; i++)
        _data[i] = value;
    }

  };
}

#endif //CPPALGORITHM_2DARRAY_H
