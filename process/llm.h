// AIHandler.h
#ifndef LLM_H
#define LLM_H

#include "../signals.h"

#include <cstring>

class AIHandler {
public:
    AIHandler(const std::string& modelPath, const std::string& adapterPath);
    static std::string GenerateResponse(const std::string& input, const std::string& modelName);
    static void HandleNewMessages();
    static const std::string primary_prompt;
    static const std::string secondary_prompt;
    static Signal signal;
private:
    static std::string loadedModelCommand;
    static std::string model_name;
    static std::string adapter_path;
};


#endif // AIHANDLER_H
