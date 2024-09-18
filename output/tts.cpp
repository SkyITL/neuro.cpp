#include "tts.h"
#include "speechapi_cxx.h"
#include <iostream>
#include <thread>
#include <mutex>
#include <queue>

// Define global variables for speech key and region
const std::string speechKey = "863f1e1615a644ab956e71c71facf573";
const std::string speechRegion = "eastasia";

// Initialize the shared synthesizer
bool TTS::speaking = false;
std::shared_ptr<Microsoft::CognitiveServices::Speech::SpeechSynthesizer> TTS::synthesizer = nullptr;
std::mutex messageMutex; // Mutex for synchronizing access to the message queue

TTS::TTS(const std::string& subscriptionKey, const std::string& region)

{
    // Set up Azure Speech Configuration
    auto config = Microsoft::CognitiveServices::Speech::SpeechConfig::FromSubscription(subscriptionKey, region);
    config->SetSpeechSynthesisVoiceName("en-US-AshleyNeural"); // Adjust voice as needed

    // Create the Speech Synthesizer
    synthesizer = Microsoft::CognitiveServices::Speech::SpeechSynthesizer::FromConfig(config);

    // Set event handlers
    synthesizer->SynthesisStarted.Connect([](const Microsoft::CognitiveServices::Speech::SpeechSynthesisEventArgs&) {
        std::cout << "TTS: Audio started..." << std::endl;
        signal.setAiSpeaking(true); // Mark AI as speaking
    });

    synthesizer->SynthesisCompleted.Connect([this](const Microsoft::CognitiveServices::Speech::SpeechSynthesisEventArgs&) {
        std::cout << "TTS: Audio finished." << std::endl;
        speaking = false;
        signal.setAiSpeaking(false); // Mark AI as done speaking
    });
}

void TTS::speak(const std::string message) {
    speaking = true;

    // Define SSML with pitch adjustment
    std::string ssml = R"(
        <speak version="1.0" xmlns="http://www.w3.org/2001/10/synthesis" xmlns:mstts="https://www.w3.org/2001/mstts" xml:lang="en-US">
            <voice name="en-US-AshleyNeural">
                <prosody pitch="+26%">)" + message + R"(</prosody>
            </voice>
        </speak>
    )";

    // Create a Speech Synthesis Result object
    auto result = synthesizer->SpeakSsmlAsync(ssml).get();

    std::cout << "TTS: Speaking: " << message << std::endl;


    if (result->Reason == Microsoft::CognitiveServices::Speech::ResultReason::SynthesizingAudioCompleted) {
        std::cout << "TTS: Audio synthesis completed." << std::endl;
    } else if (result->Reason == Microsoft::CognitiveServices::Speech::ResultReason::Canceled) {
        auto cancellation = Microsoft::CognitiveServices::Speech::SpeechSynthesisCancellationDetails::FromResult(result);
        std::cout << "TTS: Audio synthesis canceled. Reason: " << static_cast<int>(cancellation->Reason) << std::endl;
        if (cancellation->Reason == Microsoft::CognitiveServices::Speech::CancellationReason::Error) {
            std::cout << "Error details: " << cancellation->ErrorDetails << std::endl;
        }
    }

    speaking = false;
}

void TTS::HandleNewMessages() {
    while (!signal.getTerminate()) {

        if (signal.hasSpeakingText() && !signal.getAiSpeaking() && !speaking) {


            auto messages = signal.getHistoryMessages();
            if (!messages.empty()) {
                std::string message = signal.getNextSpeakingText(); // Get the first message

                // Speak the message
                speak(message);
            }
        }

        // Sleep briefly to prevent tight looping
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

void TTS::stop() {
    /*std::cout << "TTS: Stopping..." << std::endl;

    // Use StopSpeakingAsync() to stop the ongoing speech immediately
    auto stopResult = synthesizer->StopSpeakingAsync().get();

    if (stopResult->Reason == Microsoft::CognitiveServices::Speech::ResultReason::Canceled) {
        auto cancellation = Microsoft::CognitiveServices::Speech::SpeechSynthesisCancellationDetails::FromResult(stopResult);
        std::cout << "TTS: Stop requested. Reason: " << static_cast<int>(cancellation->Reason) << std::endl;
    }

    speaking = false;*/
}