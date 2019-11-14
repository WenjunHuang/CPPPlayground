#include <QtCore/QFuture>
#include<QtConcurrent>

QFuture<long> skynet(int num,int size ,int div) {
    if (size == 1) {
        return QtConcurrent::run([]{
          return 1L;
        });
    }

}
int main() {

}