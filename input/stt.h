#ifndef Stt_H
#define Stt_H

#include "../signals.h"

class STT {
private:
    //std::shared_ptr<Whisper> recorder;
    static Signal signal;
    bool enabled;
public:
    STT();
    ~STT();
    STT(const STT &stt) = delete;
    void process_text(std::string text);
    void recording_start();
    void recording_stop();
    void listen_loop();
};

void stt();

#endif  // Stt_H