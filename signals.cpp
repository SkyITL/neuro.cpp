#include "signals.h"

#include <iostream>

// Static member initialization
bool Signal::aiSpeaking = false;
bool Signal::aiThinking = false;
bool Signal::newMessage = false;
std::vector<std::string> Signal::chatMessages;
bool Signal::humanSpeaking = false;
std::vector<std::string> Signal::historyMessages;
std::queue<std::string> Signal::speakingTextQueue;
double Signal::lastMessageTime = 0.0;
bool Signal::sttReady = false;
bool Signal::ttsReady = false;
bool Signal::terminate = false;

// Constructor and Destructor
Signal::Signal() {
    humanSpeaking = false;
    aiSpeaking = false;
    aiThinking = false;
    lastMessageTime = 0.0;
    newMessage = false;
    ttsReady = false;
    sttReady = false;
    terminate = false;
}

Signal::~Signal() {}

//getters and setters
void Signal::setHumanSpeaking(bool speaking) {
    humanSpeaking = speaking;
}

void Signal::setAiSpeaking(bool speaking) {
    aiSpeaking = speaking;
}

void Signal::setAiThinking(bool thinking) {
    aiThinking = thinking;
}

void Signal::setLastMessageTime(double time) {
    lastMessageTime = time;
}

void Signal::setNewMessage(bool message) {
    newMessage = message;
}

void Signal::queueSpeakingText(const std::string& message) {
    speakingTextQueue.push(message);
}

void Signal::setTtsReady(bool ready) {
    ttsReady = ready;
}

void Signal::setSttReady(bool ready) {
    sttReady = ready;
}

void Signal::addHistoryMessage(std::string message) {
    historyMessages.push_back(message);
}

void Signal::addChatMessage(std::string message) {
    chatMessages.push_back(message);
}

void Signal::setTerminate(bool term) {
    terminate = term;
}

bool Signal::getHumanSpeaking() {
    return humanSpeaking;
}

bool Signal::getAiSpeaking() {
    return aiSpeaking;
}

bool Signal::getAiThinking() {
    return aiThinking;
}

double Signal::getLastMessageTime() {
    return lastMessageTime;
}

bool Signal::getNewMessage() {
    return newMessage;
}

bool Signal::hasSpeakingText() {
    return !speakingTextQueue.empty();
}

std::string Signal::getNextSpeakingText() {
    if (speakingTextQueue.empty()) {
        return "";
    }
    std::string next = speakingTextQueue.front();
    speakingTextQueue.pop();
    return next;
}

bool Signal::getTtsReady() {
    return ttsReady;
}

bool Signal::getSttReady() {
    return sttReady;
}

std::vector<std::string> Signal::getHistoryMessages() {
    return historyMessages;
}

std::vector<std::string> Signal::getChatMessages() {
    return chatMessages;
}

bool Signal::getTerminate() {
    return terminate;
}