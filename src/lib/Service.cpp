#include "Service.h"

Service::Service(string const &serviceName, string const &queueManager)
{
  this->serviceName = serviceName;
  this->queueManager = queueManager;
}

string Service::getServiceName() const
{
  return serviceName;
}
void Service::setServiceName(string const &serviceName)
{
  this->serviceName = serviceName;
}

string Service::getQueueManager() const
{
  return queueManager;
}
void Service::setQueueManager(string const &queueManager)
{
  this->queueManager = queueManager;
}

string Service::getPhysicalQueueName(string const &logicalName)
{
  return mapQueueNameLogicalToPhysical[logicalName];
}


map<string, string> Service::getQueueMap() const
{
  return mapQueueNameLogicalToPhysical;
}



void Service::addQueue(string const &logicalName, string const &physicalName)
{
  mapQueueNameLogicalToPhysical.insert(make_pair(logicalName, physicalName));
}

ostream &operator<<(ostream &os, const Service &sc)
{
  os << "Service name = " << sc.serviceName << "\n";
  os << "Queue Manager = " << sc.queueManager << "\n";
  os << "Queue number  = " << sc.getQueueMap().size() << "\n";
  return os;
}
