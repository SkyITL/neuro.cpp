// AIHandler.h
#ifndef LLM_H
#define LLM_H

#include "../signals.h"

#include <cstring>

class AIHandler {
public:

    // Method to handle new messages and generate LLM responses
    static void HandleNewMessages();

private:
    static Signal signal; // Reference to the Signal object

    // Method to infer response using Ollama
    static const std::string primary_prompt;
    static const std::string secondary_prompt;
    static std::string GenerateResponse(const std::string& input, const std::string& modelName);
};

#endif // AIHANDLER_H
