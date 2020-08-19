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
  os << "Service name       [" << sc.serviceName << "]\n";
  os << "Queue Manager      [" << sc.queueManager << "]\n";
  os << "Queue Labels/Names [";
  for (std::pair<std::string, std::string> element : sc.getQueueMap())
  {
    os << element.first << " - " << element.second << " | ";
  }
  os << "]\n";
  return os;
}

bool Service::operator==(const Service &service) const
{
  return getServiceName() == service.getServiceName() && getQueueManager() == service.getQueueManager();
}
