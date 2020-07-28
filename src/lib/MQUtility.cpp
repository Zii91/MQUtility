#include "MQUtility.h"

MQUtility::MQUtility::MQUtility(std::string const &filename, int const &mode)
{
  this->configurationFile = filename;
  this->mode = mode;
  this->serviceName = "";
};

MQUtility::MQUtility::MQUtility(std::string const &filename, string const &serviceName, int const &mode)
{
  this->configurationFile = filename;
  this->mode = mode;
  this->serviceName = serviceName;
};

int MQUtility::MQUtility::run()
{
  init();
  return checkConfiguration();
};

void MQUtility::MQUtility::init()
{
  switch (mode)
  {
  case MQUtilityMode::XML_MODE:
    initWithXMLSource();
    break;
  default:
    throw "Mode not supported yet";
  }
};

void MQUtility::MQUtility::initWithXMLSource()
{
  configuration = new XMLFileConfiguration(configurationFile);
  try
  {
    configuration->loadConfiguration();
  }
  catch (char *const &e)
  {
    cerr << e << endl;
    throw e;
  }
};

int MQUtility::MQUtility::checkConfiguration()
{

  if (serviceName.length() != 0)
  {
    std::cout << "Checking MQ Connection for Service = " << serviceName << std::endl;
    Service *s{configuration->getServiceConfiguration(serviceName)};
    const std::string queueManager{s->getQueueManager()};
    std::cout << "MQ Queue Manager = " << queueManager << " for above service" << std::endl;
    if (mqClient->initialise(queueManager))
    {
      MQConnection *connection{mqClient->connect(queueManager)};
      if (connection->getReturnCode() != true)
      {
        return -1;
      }
      for (std::pair<std::string, std::string> element : s->getQueueMap())
      {
        if (mqClient->openQueueConnection(connection, element.second) != true)
        {
          std::cout << "MQ_FAILURE for queue" << element.first << std::endl;
          return -1;
        }
        else
        {
          mqClient->closeQueueConnection(connection, element.second);
        }
      }
      mqClient->closeManagerConnection(connection);
      std::cout << "MQ_SUCCESS" << std::endl;
      return 1;
    }
    else
    {
      std::cout << "MQ_FAILURE" << std::endl;
      return -1;
    }
  }
  return 1;
};

void MQUtility::MQUtility::printConfiguration()
{
  if (serviceName.length() != 0)
  {
    Service *s{configuration->getServiceConfiguration("Service1")};
    cout << *s;
  }
  else
  {
    for (Service *s : configuration->getServiceConfigurationVector())
    {
      cout << *s;
    }
  }
};

MQClient *MQUtility::MQUtility::getMQClient()
{
  return mqClient;
};
void MQUtility::MQUtility::setMQClient(MQClient *const &mqClient)
{
  this->mqClient = mqClient;
};
