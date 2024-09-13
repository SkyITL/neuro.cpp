#include "llm.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <cstdio>
#include <memory>
#include <sstream>

// Function to escape special characters for shell commands
std::string escapeForShell(const std::string& input) {
    std::ostringstream escaped;
    for (char c : input) {
        switch (c) {
            case '\"': escaped << "\\\""; break;
            case '\'': escaped << "\\'"; break;
            case '\\': escaped << "\\\\"; break;
            case '\n': escaped << "\\n"; break;
            case '$': escaped << "\\$"; break;
            case '`': escaped << "\\`"; break;
            default: escaped << c;
        }
    }
    return escaped.str();
}

void AIHandler::HandleNewMessages() {
    while (!signal.getTerminate()) {
        if (signal.getNewMessage() && !signal.getAiThinking()) {
            signal.setNewMessage(false);
            auto messages = signal.getHistoryMessages();
            if (!messages.empty()) {
                std::string prompt;
                for (const auto& msg : messages) {
                    prompt += msg + "\n";
                }

                signal.setAiThinking(true);

                // Step 1: Generate initial response using primary model
                std::string primaryResponse = GenerateResponse(prompt, "llama3.1");

                printf("%s\n",primaryResponse.c_str());

                // Step 2: Refine the response using the secondary model
                std::string refinedResponse = GenerateResponse(primaryResponse, "llama3.1");

                printf("%s",refinedResponse.c_str());

                // Add the refined response to the history and queue it for speaking
                signal.addHistoryMessage("AI: " + refinedResponse);
                signal.queueSpeakingText(refinedResponse);

                signal.setAiThinking(false);
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

std::string AIHandler::GenerateResponse(const std::string& input, const std::string& modelName) {
    std::string safeInput = escapeForShell(input);
    std::string command = "echo \"" + safeInput + "\" | ollama run " + modelName;

    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(command.c_str(), "r"), pclose);
    if (!pipe) {
        std::cerr << "AIHandler: Failed to run Ollama command with model " << modelName << "." << std::endl;
        return "Sorry, I couldn't understand that.";
    }

    char buffer[128];
    std::string result;
    while (fgets(buffer, sizeof(buffer), pipe.get()) != nullptr) {
        result += buffer;
    }

    return result;
}
