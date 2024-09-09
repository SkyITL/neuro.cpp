#include "stt.h"

#include <iostream>
#include <cstring>
#include <cstdlib>

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
    std::string audioFilePath = "/Users/skyliu/Documents/GPT-SoVITS/output/asr_opt/Neuro.mp3_0000320320_0000480000.wav"; // Replace with your audio file path
    std::string command = "python3 whisper_recognize.py " + audioFilePath;

    std::cout << "STT Starting" << std::endl;
    int result = std::system(command.c_str());

    if (result == 0) {
        std::cout << "STT Completed Successfully." << std::endl;
    } else {
        std::cerr << "Error executing Whisper command." << std::endl;
    }
}

void stt() {
    std::cout << "STT module running..." << std::endl;
}