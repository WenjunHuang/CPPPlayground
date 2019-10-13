//
// Created by HUANG WEN JUN on 2019/10/12.
//
#include <variant>

template <typename... Ts> struct overloaded : Ts... {
    using Ts::operator()...;
};

template <typename... Ts> overloaded(Ts...)->overloaded<Ts...>;

class Tennis {
    enum class Points { Love, Fifteen, Thirty };

    enum class Player { Player1, Player2 };

    struct NormalScoring {
        Points player1Points;
        Points player2Points;
    };

    struct FortyScoring {
        Player leadingPlayer;
        Points otherPlayerScores;
    };

    struct Deuce {};

    struct Advantage {
        Player playerWithAdvantage;
    };

    void pointFor(Player whichPlayer) {
        std::visit(overloaded{[&](const NormalScoring& state) {},
                              [&](const FortyScoring& state) {},
                              [&](const Deuce& state) {},
                              [&](const Advantage& state) {}},
                   _state);
    }

  private:
    std::variant<NormalScoring, FortyScoring, Deuce, Advantage> _state;
};

int main(){}