#ifndef TTS_H
#define TTS_H

#include "../signals.h"

#include "speechapi_cxx.h"

#include<iostream>
#include<thread>

class TTS {
private:
    static bool speaking;
    static Signal signal;
    static std::shared_ptr<Microsoft::CognitiveServices::Speech::SpeechSynthesizer> synthesizer;
public:
    TTS(const std::string& subscriptionKey, const std::string& region);
    TTS(const TTS &tts) = delete;

    static void speak(std::string message);
    static void audio_started();
    static void audio_ended();
    static void stop();
    static void tts();
    static void HandleNewMessages();
};


#endif  // TTS_H