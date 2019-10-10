//
// Created by HUANG WEN JUN on 2019/9/11.
//

#define CATCH_CONFIG_MAIN
#include "../base/bittorrent/magnet_uri.h"
#include <catch2/catch.hpp>
TEST_CASE("abc", "[test]") {
    BitTorrent::MagnetUri magnetUri(
        QStringLiteral("04AF2550620D2322A01E1485F1A80B1A956EFB72"));
}