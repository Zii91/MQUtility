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

int MQUtility::MQUtility::checkConfiguration()
{
  int rc  = MQ_SUCCESS;
  if (serviceName.length() != 0)
  {
    rc = checkConfiguration(serviceName);
    printConfigurationReport(serviceName,rc);
  }
  else
  {
    vector<Service *> vs = configuration->getServiceConfigurationVector();
    for (auto const &service : vs)
    {
      const std::string localServiceName = service->getServiceName();
      rc = rc * checkConfiguration(localServiceName);
      printConfigurationReport(localServiceName,rc);
    }
  }
  return rc;
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

int MQUtility::MQUtility::checkConfiguration(std::string const &service)
{
  int rc = MQ_SUCCESS;
  if (service.length() != 0)
  {
    std::cout << "Checking MQ Connection for Service = " << service << std::endl;
    Service *s{configuration->getServiceConfiguration(service)};
    const std::string queueManager{s->getQueueManager()};
    if (mqClient->initialise(queueManager))
    {
      MQConnection *connection{mqClient->connect(queueManager)};
      if (connection->getReturnCode() != true)
      {
        rc = MQ_FAILURE;
      }else{
        for (std::pair<std::string, std::string> element : s->getQueueMap())
        {
          if (mqClient->openQueueConnection(connection, element.second) != true)
          {
            rc = MQ_FAILURE;
          }
          else
          {
            mqClient->closeQueueConnection(connection, element.second);
            rc = rc * MQ_SUCCESS;
          }
        }
      }
      mqClient->closeManagerConnection(connection);
    }else{
      rc = MQ_FAILURE;
    }
  }
  return rc;
};

void MQUtility::MQUtility::printConfigurationReport(std::string const& service, int const& status) const
{
  if (service.length() != 0)
  {
    Service *s{configuration->getServiceConfiguration(service)};
    cout << *s;
    if (status == MQ_SUCCESS )
    {
      cout << "Status [ MQ_SUCCESS ]" << std::endl;
    }else
    {
      cout << "Status [ MQ_FAILURE ]" << std::endl;
    }
  }
};

MQClient *MQUtility::MQUtility::getMQClient() const
{
  return mqClient;
};

void MQUtility::MQUtility::setMQClient(MQClient *const &mqClient)
{
  this->mqClient = mqClient;
};
