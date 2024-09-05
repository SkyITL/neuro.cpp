#include "stt.h"

#include <iostream>

STT::STT() {
    enabled = true;
}

STT::~STT() {}

void STT::process_text(std::string text) {
    if (!this->enabled) {
        return;
    }
    std::cout << "STT: " << text << std::endl;
}

void STT::recording_start() {
    this->signal.setHumanSpeaking(true);
}

void STT::recording_stop() {
    this->signal.setHumanSpeaking(false);
}

void STT::listen_loop() {
    printf("STT: Starting\n");
    
}

void stt() {
    std::cout << "STT module running..." << std::endl;
}