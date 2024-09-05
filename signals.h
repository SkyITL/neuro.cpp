#ifndef Signals_H
#define Signals_H

#include <vector>

class Signal {
private:
    static bool humanSpeaking;
    static bool aiSpeaking;
    static bool aiThinking;
    static double lastMessageTime;
    static bool newMessage;
    static bool ttsReady;
    static bool sttReady;

    static std::vector<std::string> historyMessages;
    static std::vector<std::string> chatMessages;

    //indication of all threads in the program to terminate
    static bool terminate;
public:
    Signal() = delete;
    ~Signal() = delete;
    Signal(const Signal &signal) = delete;

    static void setHumanSpeaking(bool);
    static void setAiSpeaking(bool);
    static void setAiThinking(bool);
    static void setLastMessageTime(double);
    static void setNewMessage(bool);
    static void setTtsReady(bool);
    static void setSttReady(bool);

    static void addHistoryMessage(std::string);
    static void addChatMessage(std::string);

    static void setTerminate(bool);

    static bool getHumanSpeaking();
    static bool getAiSpeaking();
    static bool getAiThinking();
    static double getLastMessageTime();
    static bool getNewMessage();
    static bool getTtsReady();
    static bool getSttReady();

    static std::vector<std::string> getHistoryMessages();
    static std::vector<std::string> getChatMessages();

    static bool getTerminate();
};

#endif