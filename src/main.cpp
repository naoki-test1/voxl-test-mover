#include "mapper_cmd.hpp"

std::atomic<bool> running(true);

void signalHandler_ZBFT(int signum) {//VOXL LIBS HAS ITS OWN SIGHANDLER FUNCTION, BUT IT'S NOT USED HERE
    running = false; 
}

int main() {
    std::signal(SIGINT, signalHandler_ZBFT);
    auto mapper_cmd = std::make_unique<MapperCmd>();//SMART POINTER TO AVOID MEMORY LEAKS
    mapper_cmd->initMPA();
    while (running) {
        usleep(50000);//POSIX FUNCTION --> COULD USE CHRONO INSTEAD 
    }
    return 0;
}
