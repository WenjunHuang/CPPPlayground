//
// Created by HUANG WEN JUN on 2019/8/29.
//

#include "DownloadManager.h"
#include "../Preferences.h"
#include <QNetworkCookie>
#include <QNetworkCookieJar>
#include <QNetworkReply>
#include <QTemporaryFile>

// Disguise as Firefox to avoid web server banning
const char kDefaultUserAgent[] =
    "Mozilla/5.0 (X11; Linux x86_64; rv:68.0) Gecko/20100101 Firefox/68.0";

namespace {
    const int kMaxRedirections = 20;

    class NetworkCookieJar : public QNetworkCookieJar {
      public:
        explicit NetworkCookieJar(QObject* parent = nullptr)
            : QNetworkCookieJar(parent) {
            const auto now = QDateTime::currentDateTime();
            auto cookies   = Preferences::instance()->getNetworkCookies();

            for (const QNetworkCookie& cookie :
                 Preferences::instance()->getNetworkCookies()) {
                if (cookie.isSessionCookie() ||
                    (cookie.expirationDate() <= now))
                    cookies.removeAll(cookie);
            }

            setAllCookies(cookies);
        }
        ~NetworkCookieJar() override {
            auto now                      = QDateTime::currentDateTime();
            QList<QNetworkCookie> cookies = allCookies();
            for (const auto& cookie : allCookies()) {
                if (cookie.isSessionCookie() ||
                    (cookie.expirationDate() <= now))
                    cookies.removeAll(cookie);
            }

            Preferences::instance()->setNetworkCookies(cookies);
        }

        using QNetworkCookieJar::allCookies;
        using QNetworkCookieJar::setAllCookies;

        QList<QNetworkCookie> cookiesForUrl(const QUrl& url) const override {
            auto now     = QDateTime::currentDateTime();
            auto cookies = QNetworkCookieJar::cookiesForUrl(url);
            for (auto& cookie : QNetworkCookieJar::cookiesForUrl(url)) {
                if (!cookie.isSessionCookie() ||
                    (cookie.expirationDate() <= now))
                    cookies.removeAll(cookie);
            }

            return cookies;
        }

        bool setCookiesFromUrl(const QList<QNetworkCookie>& cookieList,
                               const QUrl& url) override {
            const QDateTime now           = QDateTime::currentDateTime();
            QList<QNetworkCookie> cookies = cookieList;
            for (const QNetworkCookie& cookie : cookieList) {
                if (!cookie.isSessionCookie() &&
                    (cookie.expirationDate() <= now))
                    cookies.removeAll(cookie);
            }

            return QNetworkCookieJar::setCookiesFromUrl(cookies, url);
        }
    };

    class DownloadHandlerImpl : public Net::DownloadHandler {
        Q_DISABLE_COPY(DownloadHandlerImpl)
      public:
        explicit DownloadHandlerImpl(
            const Net::DownloadRequest& downloadRequest, QObject* parent);
        ~DownloadHandlerImpl() override;

        QString url() const;
        const Net::DownloadRequest downloadRequest() const;
        void assignNetworkReply(QNetworkReply* reply);

      private:
        void processFinishedDownload();
        void checkDownloadSize(qint64 bytesReceived, qint64 bytesTotal);
        void handleRedirection(const QUrl& newUrl);
        void setError(const QString& error);
        void finish();

        static QString errorCodeToString(QNetworkReply::NetworkError status);

        QNetworkReply* m_reply = nullptr;
        const Net::DownloadRequest m_downloadRequest;
        Net::DownloadResult m_result;
    };

    QNetworkRequest
    createNetworkRequest(const Net::DownloadRequest& downloadRequest) {
        QNetworkRequest request{downloadRequest.url()};

        if (downloadRequest.userAgent().isEmpty())
            request.setRawHeader("User-Agent", kDefaultUserAgent);
        else
            request.setRawHeader("User-Agent",
                                 downloadRequest.userAgent().toUtf8());

        // Spoof HTTP Referer to allow adding torrent link from
        // Torcache/KickAssTorrents
        request.setRawHeader("Referer", request.url().toEncoded().data());
        // Accept gzip
        request.setRawHeader("Accept-Encoding", "gzip");

        request.setAttribute(QNetworkRequest::RedirectPolicyAttribute,
                             QNetworkRequest::UserVerifiedRedirectPolicy);
        request.setMaximumRedirectsAllowed(kMaxRedirections);

        return request;
    }

    bool saveToFile(const QByteArray& replyData, QString& filePath) {
        QTemporaryFile tempFile{Utils::Fs::tempPath() + "XXXXXX"};
        tempFile.setAutoRemove(false);

        if (!tempFile.open())
            return false;

        filePath = tempFile.fileName();
        tempFile.write(replyData);
        return true;
    }
} // namespace

Net::DownloadManager* Net::DownloadManager::m_instance = nullptr;

Net::DownloadManager::DownloadManager(QObject* parent) : QObject(parent) {
    connect(&m_networkManager, &QNetworkAccessManager::sslErrors, this,
            &Net::DownloadManager::ignoreSslErrors);
    connect(&m_networkManager, &QNetworkAccessManager::finished, this,
            &DownloadManager::handleReplyFinished);
    connect(ProxyConfigurationManager::instance(),
            &ProxyConfigurationManager::proxyConfigurationChanged, this,
            &DownloadManager::applyProxySettings);
    m_networkManager.setCookieJar(new NetworkCookieJar(this));
    applyProxySettings();
}

void Net::DownloadManager::initInstance() {
    if (!m_instance)
        m_instance = new DownloadManager;
}

void Net::DownloadManager::freeInstance() {
    if (m_instance) {
        delete m_instance;
        m_instance = nullptr;
    }
}

Net::DownloadManager* Net::DownloadManager::instance() { return m_instance; }

Net::DownloadHandler*
Net::DownloadManager::download(const DownloadRequest& downloadRequest) {
    const auto request             = createNetworkRequest(downloadRequest);
    const auto id                  = ServiceID::fromURL(request.url());
    const auto isSequentialService = m_sequentialServices.contains(id);

    auto downloadHandler = new DownloadHandlerImpl{downloadRequest, this};
    connect(downloadHandler, &DownloadHandler::finished, downloadHandler,
            &QObject::deleteLater);
    connect(downloadHandler, &QObject::destroyed, this,
            [this, id, downloadHandler]() {
                m_waitingJobs[id].removeOne(downloadHandler);
            });

    if (isSequentialService && m_busyServices.contains(id)) {
        m_waitingJobs[id].enqueue(downloadHandler);
    } else {
        qDebug("Downloading %s...", qUtf8Printable(downloadRequest.url()));
        if (isSequentialService)
            m_busyServices.insert(id);
        downloadHandler->assignNetworkReply(m_networkManager.get(request));
    }

    return downloadHandler;
}

void Net::DownloadManager::registerSequentialService(
    const Net::ServiceID& serviceID) {
    m_sequentialServices.insert(serviceID);
}

QList<QNetworkCookie>
Net::DownloadManager::cookiesForUrl(const QUrl& url) const {
    return m_networkManager.cookieJar()->cookiesForUrl(url);
}

bool Net::DownloadManager::setCookiesFromUrl(
    const QList<QNetworkCookie>& cookieList, const QUrl& url) {
    return m_networkManager.cookieJar()->setCookiesFromUrl(cookieList, url);
}

QList<QNetworkCookie> Net::DownloadManager::allCookies() const {
    return static_cast<NetworkCookieJar*>(m_networkManager.cookieJar())
        ->allCookies();
}

void Net::DownloadManager::setAllCookies(
    const QList<QNetworkCookie>& cookieList) {
    static_cast<NetworkCookieJar *>(m_networkManager.cookieJar())->setAllCookies(cookieList);
}
