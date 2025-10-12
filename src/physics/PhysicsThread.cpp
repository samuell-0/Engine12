#include "physics/PhysicsThread.h"
#include "window/State.hpp"
#include <iostream>
void DoPhysics(void* const appstate){
    AppState* as = (AppState*)appstate;
    while(!as->signal_to_exit){
        std::cout << "doing physics!" << std::endl;
    }
}