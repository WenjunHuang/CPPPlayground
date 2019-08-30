//
// Created by HUANG WEN JUN on 2019/8/30.
//

#pragma once
#include <QObject>

namespace Net {
    enum class ProxyType {
        None      = 0,
        HTTP      = 1,
        SOCKS5    = 2,
        HTTP_PW   = 3,
        SOCKS5_PW = 4,
        SOCKS4    = 5
    };

    struct ProxyConfiguration {
        ProxyType type = ProxyType::None;
        QString ip     = "0.0.0.0";
        qint16 port    = 8080;
        QString username;
        QString password;
    };

    bool operator==(const ProxyConfiguration& left,
                    const ProxyConfiguration& right);
    bool operator!=(const ProxyConfiguration& left,
                    const ProxyConfiguration& right);

    class ProxyConfigurationManager : public QObject {
        Q_OBJECT
        Q_DISABLE_COPY(ProxyConfigurationManager)

        explicit ProxyConfigurationManager(QObject* parent = nullptr);
        ~ProxyConfigurationManager() = default;

      public:
        static void initInstance();
        static void freeInstance();
        static ProxyConfigurationManager* instance();

        ProxyConfiguration proxyConfiguration() const;
        void setProxyConfiguration(const ProxyConfiguration& config);

        bool proxyOnlyForTorrents() const;
        void setProxyOnlyForTorrents(bool value);

        bool authenticationRequired() const;

      signals:
        void proxyConfigurationChanged();

      private:
        void configureProxy();

        static ProxyConfigurationManager* m_instance;
        ProxyConfiguration m_config;
        bool m_isProxyOnlyForTorrents;
    };
} // namespace Net
