//
// Created by rick on 2019/9/11.
//

#pragma once

class TriStateBool {
  public:
    static const TriStateBool Undefined;
    static const TriStateBool False;
    static const TriStateBool True;

    constexpr TriStateBool()                          = default;
    constexpr TriStateBool(const TriStateBool& other) = default;
    explicit constexpr TriStateBool(int value)
        : m_value(value < 0 ? -1 : (value > 0 ? 1 : 0)) {}

    explicit constexpr operator int() const { return m_value; }

    TriStateBool& operator=(const TriStateBool& other) = default;

    constexpr friend bool operator==(const TriStateBool& left,
                                     const TriStateBool& right) {
        return left.m_value == right.m_value;
    }

  private:
    signed char m_value = -1;
};

constexpr bool operator!=(const TriStateBool& left, const TriStateBool& right) {
    return !(left == right);
}