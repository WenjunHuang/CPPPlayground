//
// Created by xxzyjy on 2018/1/9.
//

#include <QCoreApplication>
#include <QCommandLineParser>
#include <QtNetwork>

struct DnsQuery {
  DnsQuery() : type(QDnsLookup::A) {}

  QDnsLookup::Type type;
  QHostAddress nameServer;
  QString name;
};

enum class CommandLineParseResult {
  CommandLineOk,
  CommandLineError,
  CommandLineVersionRequested,
  CommandLineHelpRequested
};

QTextStream &qStdErr() {
  static QTextStream ts(stderr);
  return ts;
}

QDnsLookup::Type typeFromParameter(const QString &qString) {
  if (qString == "a")
    return QDnsLookup::A;
  else if (qString == "aaaa")
    return QDnsLookup::AAAA;
  else if (qString == "any")
    return QDnsLookup::ANY;
  else if (qString == "cname")
    return QDnsLookup::CNAME;
  else if (qString == "mx")
    return QDnsLookup::MX;
  else if (qString == "ns")
    return QDnsLookup::NS;
  else if (qString == "ptr")
    return QDnsLookup::PTR;
  else if (qString == "srv")
    return QDnsLookup::SRV;
  else if (qString == "txt")
    return QDnsLookup::TXT;
  return (QDnsLookup::Type) -1;
}

CommandLineParseResult parseCommandLine(QCommandLineParser &parser,
                                        DnsQuery *query,
                                        QString *errorMessage) {
  parser.setSingleDashWordOptionMode(QCommandLineParser::ParseAsLongOptions);
  const QCommandLineOption nameServerOption{"n", "The name server to used.", "nameserver"};
  parser.addOption(nameServerOption);

  const QCommandLineOption typeOption{"t", "The lookup type.", "type"};
  parser.addOption(typeOption);

  parser.addPositionalArgument("name", "The name to look up.");

  const QCommandLineOption helpOption = parser.addHelpOption();
  const QCommandLineOption versionOption = parser.addVersionOption();

  if (!parser.parse(QCoreApplication::arguments())) {
    *errorMessage = parser.errorText();
    return CommandLineParseResult::CommandLineError;
  }

  if (parser.isSet(versionOption))
    return CommandLineParseResult::CommandLineVersionRequested;

  if (parser.isSet(helpOption))
    return CommandLineParseResult::CommandLineHelpRequested;

  if (parser.isSet(nameServerOption)) {
    const auto nameserver = parser.value(nameServerOption);
    query->nameServer = QHostAddress(nameserver);
    if (query->nameServer.isNull() || query->nameServer.protocol() == QAbstractSocket::UnknownNetworkLayerProtocol) {
      *errorMessage = QString("Bad nameserver address: %1").arg(nameserver);
      return CommandLineParseResult::CommandLineError;
    }
  }

  if (parser.isSet(typeOption)) {
    const auto typeParameter = parser.value(typeOption);
    const auto type = typeFromParameter(typeParameter.toLower());
    if (type < 0) {
      *errorMessage = QString("Bad record type: %1").arg(typeParameter);
      return CommandLineParseResult::CommandLineError;
    }
  }

  const auto positionalArguments = parser.positionalArguments();
  if (positionalArguments.isEmpty()) {
    *errorMessage = "Argument 'name' missing.";
    return CommandLineParseResult::CommandLineError;
  }
  if (positionalArguments.size() > 1) {
    *errorMessage = "Several 'name' arguments specified.";
    return CommandLineParseResult::CommandLineError;
  }

  query->name = positionalArguments.first();
  return CommandLineParseResult::CommandLineOk;
}

int main(int argc, char *argv[]) {
  QCoreApplication app(argc, argv);
  QCoreApplication::setApplicationName(QCoreApplication::tr("QDnsLookupExample", "DNS Lookup Example"));
  QCoreApplication::setApplicationVersion(QT_VERSION_STR);

  QCommandLineParser parser;
  parser.setApplicationDescription(
    QCoreApplication::tr("QDnsLookupExample", "An example demonstrating the class QDnsLookup."));
  DnsQuery query;
  QString errorMessage;

  switch (parseCommandLine(parser, &query, &errorMessage)) {
    case CommandLineParseResult::CommandLineOk:
      break;
    case CommandLineParseResult::CommandLineError:
      qStdErr() << errorMessage << "\n\n" << parser.helpText();
      return 1;
    case CommandLineParseResult::CommandLineVersionRequested:
      qStdErr() << QString("%1 %1\n")
        .arg(QCoreApplication::applicationName())
        .arg(QCoreApplication::applicationVersion());
      return 0;
    case CommandLineParseResult ::CommandLineHelpRequested:
      parser.showHelp();
      Q_UNREACHABLE();
  }
}
