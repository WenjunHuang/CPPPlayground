//
// Created by xxzyjy on 01/10/2017.
//

#include <string>
#include <iostream>
#include <caf/all.hpp>

using std::endl;
using std::string;

using namespace caf;

behavior mirror(event_based_actor *self) {
  return {
    [=](const string& what) -> string {
      aout(self) << what << endl;
      return string(what.rbegin(),what.rend());
    }
  };
}

void hello_world(event_based_actor *self,const actor &buddy){
  self->request(buddy, std::chrono::seconds(10), "Hello World!").then(
    [=](const string& what){
      aout(self)<<what<<endl;
    }
  );
}

int main(void){
  actor_system_config cfg;
  actor_system system{cfg};

  auto mirror_actor = system.spawn(mirror);
  system.spawn(hello_world, mirror_actor);
}
