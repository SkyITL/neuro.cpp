#include "signals.h"
#include "input/stt.h"
#include "process/llm.h"
#include "output/tts.h"
#include "output/vtubeStudio.h"

#include <iostream>
#include <thread>

int main() {

    printf("Starting Project...");

    // Set up signal handler for Ctrl+C
    // signal(SIGINT, [](int) {
    //     std::cout << "Exiting..." << std::endl;
    //     exit(0);
    // });
    // // Set up signal handler for termination signal
    // signal(SIGTERM, [](int) { std::cout << "Exiting..." << std::endl; });

    // create threads for each module
    std::thread ttsThread(TTS::tts);
    std::thread llmThread(llm);
    std::thread sttThread(stt);

    // join all the threads
    ttsThread.join();
    llmThread.join();
    sttThread.join();

    return 0;
}

