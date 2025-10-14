#include <SDL3/SDL_main.h>
#include <sys/inotify.h>

#include "State.hpp"
#include <iostream>
// NOTE: should not modify state and the pointer passed to it
void watchFiles(const void* const appstate){
    AppState* as = (AppState*)appstate;
    while (!as->signal_to_exit){
        std::cout << "on the look out!" << std::endl;
    }
    std::cout << as->signal_to_exit << std::endl;
}