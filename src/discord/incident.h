// Incident.h
#ifndef Incident_H
#define Incident_H

#include <string>  // For using std::string

enum class IncidentType {
    Fire,
    Ems,
    Other
};

class Incident {
private:
    std::string RAW_MSG; // Raw unprocessed msg
    IncidentType TYPE;

public:
    // Constructor
    Incident(const std::string& msgInput);

    // Getter methods
    std::string getMsg() const;

    // Setter methods
    void setType(IncidentType type);

    // Method to display Incident's details
    void display() const;
};

#endif // Incident_H
