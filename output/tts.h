#ifndef TTS_H
#define TTS_H

#include<iostream>

class TTS {
private:
    static bool speaking;
public:
    TTS() = delete;
    ~TTS() = delete;
    TTS(const TTS &tts) = delete;

    static void speak(std::string message);
    static void stop();
    static void tts();
};

void tts();

#endif  // TTS_H