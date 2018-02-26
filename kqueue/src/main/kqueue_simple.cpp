#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/event.h>
#include <errno.h>
#include <string.h>
#include <catch.hpp>

void turn_on_flags(int fd, int flags) {
  int current_flags;

  if ((current_flags = fcntl(fd,F_GETFL)) < 0) exit(1);

  current_flags |= flags;
  if(fcntl(fd,F_SETFL,current_flags) < 0) exit(1);
}

int quit(const char* msg){
  perror(msg);
  exit(1);
}

const static int FD_NUM = 2;
const static int BUFFER_SIZE = 1024;

TEST_CASE("kqueue_simple","[kqueue]") {
  struct kevent changes[FD_NUM];
  struct kevent events[FD_NUM];

  int kq;
  if ((kq = kqueue()) == -1) quit("kqueue()");

  int stdin_fd = STDIN_FILENO;
  int stdout_fd = STDOUT_FILENO;

  turn_on_flags(stdin_fd,O_NONBLOCK);
  turn_on_flags(stdout_fd,O_NONBLOCK);

  int k = 0;
  EV_SET(&changes[k++],stdin_fd,EVFILT_READ,EV_ADD|EV_ENABLE,0,0,&stdin_fd);
  EV_SET(&changes[k++],stdout_fd,EVFILT_WRITE,EV_ADD|EV_ENABLE,0,0,&stdout_fd);

  int nev,nread,nwrote = 0;
  char buffer[BUFFER_SIZE];
  while(1) {
    nev = kevent(kq,changes,FD_NUM,events,FD_NUM,NULL);
    if (nev<=0)quit("kevet()");

    for (int i = 0;i<nev;i++) {
      kevent event = events[i];
      if (event.flags & EV_ERROR) quit("Event error");

      int ev_fd = *((int *)event.udata);
      if (ev_fd == stdin_fd && nread < BUFFER_SIZE) {
        if new_nread;

      }
    }
  }
}

