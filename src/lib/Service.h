#pragma once
#include <string>
#include <map>
#include <iostream>
using namespace std;

class Service
{
  string serviceName;
  string queueManager;
  //I used a map to store queues logical and physical names
  // If only physical name should be stored/used a vector could've been better for this case.
  map<string, string> mapQueueNameLogicalToPhysical;

public:
  Service(string const &serviceName, string const &queueManager);

  //Getters & setters
  string getServiceName() const;
  void setServiceName(string const &serviceName);

  string getQueueManager() const;
  void setQueueManager(string const &queueManager);

  string getPhysicalQueueName(string const &logicalName);
  void addQueue(string const &logicalName, string const &physicalName);

  map<string, string> getQueueMap() const;
  bool operator == (const Service &service) const;
  friend std::ostream& operator <<(std::ostream&, const Service&);

  ~Service();
};

