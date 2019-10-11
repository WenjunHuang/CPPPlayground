
#include <utility>
class error_test_t{
  public:
    error_test_t(bool error = true):_error{error}{}

    template<typename T>
    bool operator()(T&& value) const {
        return _error == std::forward<T>(value).error();
    }

    error_test_t operator==(bool test){
        return error_test_t(test? _error:!_error);
    }
  private:
    bool _error;
};