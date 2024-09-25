#include "signals.h"
#include "input/stt.h"
#include "process/llm.h"
#include "output/tts.h"
#include "output/vtubeStudio.h"

#include <iostream>
#include <thread>

/*int main() {

    printf("Starting Project...");

    const std::string speechKey = "";
    const std::string speechRegion = "";
    printf(Signal::getTerminate()? "True\n":"False\n");

    // Create TTS object
    TTS tts(speechKey, speechRegion);
    AIHandler llm("llama3.1_dolphin_8B","llama3.1_8B/adapters");

    // Launch the TTS::HandleNewMessages() function in a separate thread
    std::thread ttsThread(&TTS::HandleNewMessages); 
    // Launch the STT::listen_loop() function in a separate thread
    std::thread sttThread(&STT::listen_loop);
    // Launch the AIHandler::HandleNewMessages() function in a separate thread
    std::thread llmThread(&AIHandler::HandleNewMessages);

    // Join the threads
    sttThread.join();
    llmThread.join();
    ttsThread.join();

    // tts.speak("Hello, this is a test of the Azure Text-to-Speech service.");

    // Clean up the thread
    /*if (ttsThread.joinable()) {
        ttsThread.detach(); // Detach if you don't want to join to prevent blocking, or use join() if you want to wait for completion.
    }
    
    printf(Signal::getTerminate()? "True\n":"False\n");
    while (!Signal::getTerminate()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    /*
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
*/
