// Incident.cpp
#include "incident.h"
#include <iostream> // For using std::cout

// Constructor definition
Incident::Incident(const std::string& msgInput) {
    this->RAW_MSG = msgInput;
}

// Getter for msg
std::string Incident::getMsg() const {
    return RAW_MSG;
}

// Setter for incident type
void Incident::setType(IncidentType type) {
    this->TYPE = type;
}

// Method to display Incident's details
void Incident::display() const {
    std::cout << "RAW MSG: " << RAW_MSG << "\n";
}
