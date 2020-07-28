#pragma once
#include <string>
#include <map>
#include <iostream>
using namespace std;

class Service
{
  string serviceName;
  string queueManager;
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

  friend std::ostream& operator <<(std::ostream&, const Service&);
};

