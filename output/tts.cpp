#include "tts.h"

#include "speechapi_cxx.h"

#include <iostream>
#include <thread>

const std::string subscriptionKey = "YourSubscriptionKey";  // Your actual subscription key
const std::string serviceRegion = "YourServiceRegion";      // Your service region, e.g., "westus"


TTS::TTS(const std::string& subscriptionKey, const std::string& region) {
    // Set up Azure Speech Configuration
    /*auto config = SpeechConfig::FromSubscription(subscriptionKey, region);
    config->SetSpeechSynthesisVoiceName("en-US-JennyNeural"); // Adjust voice as needed

    // Create the Speech Synthesizer
    synthesizer = SpeechSynthesizer::FromConfig(config);

    // Set event handlers
    synthesizer->SynthesisStarted.Connect([](const SpeechSynthesisEventArgs&) {
        std::cout << "TTS: Audio started..." << std::endl;
        speaking = true;
    });

    synthesizer->SynthesisCompleted.Connect([](const SpeechSynthesisEventArgs&) {
        std::cout << "TTS: Audio finished." << std::endl;
        speaking = false;
    });

    synthesizer->SynthesisCanceled.Connect([](const SpeechSynthesisCanceledEventArgs& e) {
        std::cerr << "TTS: Synthesis canceled. Reason=" << static_cast<int>(e.Reason) << std::endl;
        if (e.Reason == CancellationReason::Error) {
            std::cerr << "Error details: " << e.ErrorDetails << std::endl;
        }
        speaking = false;
    });*/
}

void TTS::speak(std::string message) {
    std::cout << "TTS: Speaking: " << message << std::endl;
    speaking = true;
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    speaking = false;
}

void TTS::stop() {
    std::cout << "TTS: Stopping..." << std::endl;
    speaking = false;
}

void TTS::tts() {
    std::cout << "TTS module running..." << std::endl;
    while (true) {
        if (speaking) {
            std::cout << "TTS: Speaking..." << std::endl;
        } else {
            std::cout << "TTS: Waiting for input..." << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

bool TTS::speaking = false;
