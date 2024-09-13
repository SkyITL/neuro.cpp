#ifndef Signals_H
#define Signals_H

#include <vector>
#include <queue>

class Signal { //static class to store signals for the program
private:
    static bool humanSpeaking;
    static bool aiSpeaking;
    static bool aiThinking;
    static double lastMessageTime;
    static bool newMessage;
    static bool ttsReady;
    static bool sttReady;

    //speakingTextQueue stores all text that needs to be spoken

    static std::queue<std::string> speakingTextQueue;

    //historyMessages stores all messages that have been sent
    static std::vector<std::string> historyMessages;

    //chatMessages stores all messages that have been received
    static std::vector<std::string> chatMessages;

    //indication of all threads in the program to terminate
    static bool terminate;
public:
    Signal();
    ~Signal();

    //setters

    static void setHumanSpeaking(bool);
    static void setAiSpeaking(bool);
    static void setAiThinking(bool);
    static void setLastMessageTime(double);
    static void setNewMessage(bool);
    static void setTtsReady(bool);
    static void setSttReady(bool);

    //methods to handle history and chat messages

    static void addHistoryMessage(std::string);
    static void addChatMessage(std::string);

    //method to terminate all threads

    static void setTerminate(bool);

    //getters

    static bool getHumanSpeaking();
    static bool getAiSpeaking();
    static bool getAiThinking();
    static double getLastMessageTime();
    static bool getNewMessage();
    static bool getTtsReady();
    static bool getSttReady();

    //methods to handle speaking text

    static void queueSpeakingText(const std::string& message);
    static bool hasSpeakingText();
    static std::string getNextSpeakingText();

    //methods to handle history and chat messages

    static std::vector<std::string> getHistoryMessages();
    static std::vector<std::string> getChatMessages();

    static bool getTerminate();
};

#endif  // Signals_H