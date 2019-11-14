//
// Created by rick on 2019/11/9.
//
#include <QtWidgets>
#include <QtConcurrent>
#include <QDebug>

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);

  const int iterations = 20;
  QVector<int> vector;
  for (int i = 0; i < iterations; i++)
    vector.append(i);

  QProgressDialog dialog;
  dialog.setLabelText(QString("Progressing using %1 thread(s)...")
                        .arg(QThread::idealThreadCount()));

  // Create a QFutureWatcher and connect signals and slots.
  QFutureWatcher<void> futureWatcher;
  QObject::connect(&futureWatcher, &QFutureWatcher<void>::finished, [&]() {
      qDebug() << "finished";
      dialog.reset();
  });
//  QObject::connect(&dialog, &QProgressDialog::canceled, &futureWatcher,
//                   &QFutureWatcher<void>::cancel);
//  QObject::connect(&futureWatcher,
//                   &QFutureWatcher<void>::progressRangeChanged, &dialog,
//                   &QProgressDialog::setRange);
//  QObject::connect(&futureWatcher,
//                   &QFutureWatcher<void>::progressValueChanged, &dialog,
//                   &QProgressDialog::setValue);

  // Our function to compute
  std::function<void(int&)> spin = [](int& iteration) {
      const long work = 1000 * 1000 * 400;

      volatile long v = 0;
      for (long j = 0; j < work; ++j)
        ++v;

      qDebug() << "iteration" << iteration << "in thread"
               << QThread::currentThreadId();
  };

  // Start the computation.
  auto f = QtConcurrent::map(vector, spin);
  futureWatcher.setFuture(f);

  // Display the dialog and start the event loop.
  dialog.exec();

  futureWatcher.waitForFinished();

  // Query the future to check if was canceled.
  qDebug() << "Canceled?" << futureWatcher.future().isCanceled();

}