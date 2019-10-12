//
// Created by HUANG WEN JUN on 2019/10/12.
//
#include <variant>
class Tennis {
    enum class Points {
        Love,
        Fifteen,
        Thirty
    };

    enum class Player {
        Player1,
        Player2
    };

    struct NormalScoring {
        Points player1Points;
        Points player2Points;
    };

    struct FortyScoring{
        Player leadingPlayer;
        Points otherPlayerScores;
    };

    struct Deuce{};

    struct Advantage{
        Player playerWithAdvantage;
    };

  private:
    std::variant<
};