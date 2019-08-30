//
// Created by HUANG WEN JUN on 2019/8/30.
//

#include "ProxyConfigurationManager.h"

#define SETTINGS_KEY(name) QStringLiteral("Network/Proxy/" name)
const QString KEY_ONLY_FOR_TORRENTS = SETTINGS_KEY("OnlyForTorrents");
const QString KEY_TYPE = SETTINGS_KEY("Type");
const QString KEY_IP = SETTINGS_KEY("IP");
const QString KEY_PORT = SETTINGS_KEY("Port");
const QString KEY_USERNAME = SETTINGS_KEY("Username");
const QString KEY_PASSWORD = SETTINGS_KEY("Password");

namespace {
    inline Settings
}
