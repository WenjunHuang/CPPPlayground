//
// Created by HUANG WEN JUN on 2019/8/29.
//

#pragma once
#include <QHash>
#include <QNetworkAccessManager>
#include <QObject>
#include <QQueue>
#include <QSet>

class QNetworkCookie;
class QNetworkReply;
class QSslError;
class QUrl;

namespace Net {
    struct ServiceID {
        QString hostName;
        int port;

        static ServiceID fromURL(const QUrl& url);
    };

    uint qHash(const ServiceID& serviceID, uint seed);
    bool operator==(const ServiceID& lhs, const ServiceID& rhs);

    enum class DownloadStatus { Success, RedirectedToMagnet, Failed };

    class DownloadRequest {
      public:
        DownloadRequest(const QString& url);
        DownloadRequest(const DownloadRequest& other) = default;

        QString url() const;
        DownloadRequest& url(const QString& value);

        QString userAgent() const;
        DownloadRequest& userAgent(const QString& value);

        qint64 limit() const;
        DownloadRequest& limit(qint64 value);

        bool saveToFile() const;
        DownloadRequest& saveToFile(bool value);

      private:
        QString m_url;
        QString m_userAgent;
        qint64 m_limit    = 0;
        bool m_saveToFile = false;
    };

    struct DownloadResult {
        QString url;
        DownloadStatus status;
        QString errorString;
        QByteArray data;
        QString filePath;
        QString magnet;
    };

    class DownloadHandler : public QObject {
        Q_OBJECT
        Q_DISABLE_COPY(DownloadHandler)
      public:
        using QObject::QObject;

      signals:
        void finished(const DownloadResult& result);
    };

    class DownloadManager : public QObject {
        Q_OBJECT
        Q_DISABLE_COPY(DownloadManager)

      public:
        static void initInstance();
        static void freeInstance();
        static DownloadManager* instance();

        template <typename Context, typename Func>
        void download(const DownloadRequest& downloadRequest, Context context,
                      Func&& slot);

        void registerSequentialService(const ServiceID& serviceID);

        QList<QNetworkCookie> cookiesForUrl(const QUrl& url) const;
        bool setCookiesFromUrl(const QList<QNetworkCookie>& cookieList,
                               const QUrl& url);
        QList<QNetworkCookie> allCookies() const;
        void setAllCookies(const QList<QNetworkCookie>& cookieList);
        bool deleteCookie(const QNetworkCookie& cookie);

        static bool hasSupportedScheme(const QString& url);

      private slots:
        void ignoreSslErrors(QNetworkReply*, const QList<QSslError>&);

      private:
        explicit DownloadManager(QObject* parent = nullptr);

        DownloadHandler * download(const DownloadRequest &downloadRequest);
        void applyProxySettings();
        void handleReplyFinished(const QNetworkReply* reply);

        static DownloadManager* m_instance;
        QNetworkAccessManager m_networkManager;

        QSet<ServiceID> m_sequentialServices;
        QSet<ServiceID> m_busyServices;
        QHash<ServiceID,QQueue<DownloadHandler *>> m_waitingJobs;
    };

    template <typename Context,typename Func>
    void DownloadManager::download(const DownloadRequest& downloadRequest,
        Context context,Func &&slot) {
        const auto handler = download(downloadRequest);
        connect(handler,&DownloadHandler::finished,context,slot);
    }
} // namespace Net
