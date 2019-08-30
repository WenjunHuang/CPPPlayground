//
// Created by HUANG WEN JUN on 2019/8/30.
//

#pragma once
#include <QObject>
#include <QReadWriteLock>
#include <QString>
#include <QVector>
#include <boost/circular_buffer.hpp>

const int kMaxLogMessages = 20000;

namespace Log {
    enum MsgType {
        ALL      = -1,
        NORMAL   = 0x1,
        INFO     = 0x2,
        WARNING  = 0x4,
        CRITICAL = 0x8
    };
    Q_DECLARE_FLAGS(MsgTypes, MsgType)

    struct Msg {
        int id;
        qint64 timestamp;
        MsgType type;
        QString message;
    };

    struct Peer {
        int id;
        qint64 timestamp;
        QString ip;
        bool blocked;
        QString reason;
    };
} // namespace Log

Q_DECLARE_OPERATORS_FOR_FLAGS(Log::MsgTypes)
class Logger : public QObject {
    Q_OBJECT
    Q_DISABLE_COPY(Logger)

  public:
    static void initInstance();
    static void freeInstance();
    static Logger* instance();

    void addMessage(const QString& message,
                    const Log::MsgType& type = Log::NORMAL);
    void addPeer(const QString& ip, bool blocked, const QString& reason = {});
    QVector<Log::Msg> getMessages(int lastKnownId = -1) const;
    QVector<Log::Peer> getPeers(int lastKnownId = -1) const;

  signals:
    void newLogMessage(const Log::Msg& message);
    void newLogPeer(const Log::Peer& peer);

  private:
    Logger();
    ~Logger() = default;

    static Logger* m_instance;
    boost::circular_buffer_space_optimized<Log::Msg> m_messages;
    boost::circular_buffer_space_optimized<Log::Peer> m_peers;
    mutable QReadWriteLock m_lock;
    int m_msgCounter  = 0;
    int m_peerCounter = 0;
};

void LogMsg(const QString& message,const Log::MsgType &type = Log::NORMAL);
