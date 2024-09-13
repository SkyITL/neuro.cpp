#ifndef Stt_H
#define Stt_H

#include "../signals.h"

class STT {
private:
    static bool enabled;           
    static bool analyzeRequested;              // Static flag to check if STT is enabled
    static Signal signal;                        // Static instance of Signal class for signaling
    static std::vector<float> audioBuffer;       // Static buffer to store audio samples
    static std::string currentText;              // Static string to accumulate recognized text
    static bool isSilent(const std::vector<float> &buffer);  // Static function to detect silence
    static std::string recognize(const std::vector<float> &audio); // Static function to call Whisper

public:
    static void initialize();            // Initialize PortAudio and set up
    static void terminate();             // Terminate PortAudio
    static void process_text(const std::string &text);
    static void recording_start();
    static void recording_stop();
    static void processAudioInput(const float *input, size_t frames);
    static void listen_loop();
    static void analyzeCollectedAudio();
};


#endif  // Stt_H