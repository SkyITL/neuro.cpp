#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <memory>
#include <vector>

#include <portaudio.h>
#include <pa_mac_core.h>

#include "stt.h"

#define SAMPLE_RATE 16000
#define FRAMES_PER_BUFFER 1024
#define NUM_CHANNELS 1

// Define static members
bool STT::enabled = true;
Signal STT::signal;
std::vector<float> STT::audioBuffer;
std::string STT::currentText;
bool STT::analyzeRequested = false;  // New flag to check if analysis is requested

static int recordCallback(const void *inputBuffer, void *outputBuffer,
                          unsigned long framesPerBuffer,
                          const PaStreamCallbackTimeInfo *timeInfo,
                          PaStreamCallbackFlags statusFlags,
                          void *userData) {
    const float *input = static_cast<const float*>(inputBuffer);

    // Process the audio input statically
    STT::processAudioInput(input, framesPerBuffer);
    return paContinue;
}

void STT::initialize() {
    // Initialize PortAudio
    PaError err = Pa_Initialize();
    if (err != paNoError) {
        std::cerr << "Failed to initialize PortAudio: " << Pa_GetErrorText(err) << std::endl;
    }
}

void STT::terminate() {
    // Terminate PortAudio
    Pa_Terminate();
    //signal.setTerminate(true);
    printf("Terminated\n");
    printf(signal.getTerminate()? "True\n":"False\n");
}

void STT::process_text(const std::string &text) {
    if (!enabled) return;
    std::cout << "STT: " << text << std::endl;
}

void STT::recording_start() {
    signal.setHumanSpeaking(true);
}

void STT::recording_stop() {
    signal.setHumanSpeaking(false);
}

void STT::processAudioInput(const float *input, size_t frames) {
    // Append incoming audio frames to the buffer

    // Check if this audio fragment contains speech or is silent
    bool speechDetected = !isSilent(std::vector<float>(input, input + frames));

    if (speechDetected) {
        audioBuffer.insert(audioBuffer.end(), input, input + frames);
        printf("isspeech\n");}
    else 
        printf("notspeech\n");
}


bool STT::isSilent(const std::vector<float> &buffer) {
    // Basic silence detection based on energy threshold
    float energy = 0.0f;
    for (float sample : buffer) {
        energy += sample * sample;
    }
    energy /= buffer.size();
    return energy < 0.0007;  // Adjust this threshold as needed
}


std::string STT::recognize(const std::vector<float> &audio) {
    // Write audio data to a temporary file
    const char *tempAudioFile = "/tmp/audio_input.raw";
    FILE *audioFile = fopen(tempAudioFile, "wb");
    if (!audioFile) {
        std::cerr << "STT: Failed to open temporary file for audio input." << std::endl;
        return "";
    }

    // Write the audio data to the file
    fwrite(audio.data(), sizeof(float), audio.size(), audioFile);
    fclose(audioFile);

    // Command to run the Python script, reading from the temporary audio file
    std::string command = "python3 ../input/whisper_recognize.py " + std::string(tempAudioFile);

    // Open a pipe for reading the Python script output
    FILE *readPipe = popen((command + " 2>&1").c_str(), "r");
    if (!readPipe) {
        std::cerr << "STT: Failed to open pipe for reading from Whisper command." << std::endl;
        return "";
    }

    // Capture result and error messages from Python process
    char buffer[128];
    std::string result = "";
    while (fgets(buffer, sizeof(buffer), readPipe) != nullptr) {
        result += buffer;
    }

    // Close the read pipe and check the exit status
    int status = pclose(readPipe);
    if (status == -1) {
        std::cerr << "STT: Error occurred while closing the read pipe." << std::endl;
    } else if (WEXITSTATUS(status) != 0) {
        std::cerr << "STT: Whisper command failed with exit status: " << WEXITSTATUS(status) << std::endl;
        std::cerr << "STT: Error output: " << result << std::endl;
        return "";
    }

    return result;
}



void STT::analyzeCollectedAudio() {
    // Check if there's audio to process and user has requested analysis
    if (audioBuffer.empty() || !analyzeRequested) return;
    printf("Analyzing collected audio...\n");

    // Process and recognize the collected audio
    std::string result = recognize(audioBuffer);
    if (!result.empty()) {

        printf("Analyzed..\n");
        // Add the recognized text to the history message
        signal.addHistoryMessage("Vedal: " + result);
        signal.setNewMessage(true);
        printf("Vedal: %s\n", result.c_str());
    }

    // Clear the buffer after processing
    audioBuffer.clear();
    analyzeRequested = false;  // Reset the flag
}

void STT::listen_loop() {
    std::cout << "STT: Listening from microphone..." << std::endl;

    // Initialize PortAudio
    initialize();

    // Open input stream
    PaStream *stream;
    PaError err = Pa_OpenDefaultStream(&stream, NUM_CHANNELS, 0, paFloat32, SAMPLE_RATE,
                                       FRAMES_PER_BUFFER, recordCallback, nullptr);

    if (err != paNoError) {
        std::cerr << "Failed to open PortAudio stream: " << Pa_GetErrorText(err) << std::endl;
        terminate();
        return;
    }

    err = Pa_StartStream(stream);
    if (err != paNoError) {
        std::cerr << "Failed to start PortAudio stream: " << Pa_GetErrorText(err) << std::endl;
        Pa_CloseStream(stream);
        terminate();
        return;
    }

    std::cout << "STT: Recording. Press 'a' to analyze collected audio or 'q' to quit..." << std::endl;
    char userInput;
    while (std::cin >> userInput) {
        if (userInput == 'a') {
            analyzeRequested = true;  // Set flag to request analysis
            analyzeCollectedAudio();  // Call to analyze the collected audio
        } else if (userInput == 'q') {
            break;  // Exit the loop
        }
    }

    // Stop stream
    err = Pa_StopStream(stream);
    if (err != paNoError) {
        std::cerr << "Failed to stop PortAudio stream: " << Pa_GetErrorText(err) << std::endl;
    }

    err = Pa_CloseStream(stream);
    if (err != paNoError) {
        std::cerr << "Failed to close PortAudio stream: " << Pa_GetErrorText(err) << std::endl;
    }

    terminate();
}

void stt() {
    std::cout << "STT module running..." << std::endl;
}
