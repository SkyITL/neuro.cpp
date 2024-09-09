#include "llm.h"
#include <iostream>
#include <thread>

void llm() {
    std::cout << "LLM module running..." << std::endl;
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        std::cout << "LLM: Waiting for input..." << std::endl;
        std::cout << "LLM: Received input: " << std::endl;
    }
}