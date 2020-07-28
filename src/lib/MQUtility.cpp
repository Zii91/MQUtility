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

void MQUtility::MQUtility::run()
{
  init();
  checkConfiguration();
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

void MQUtility::MQUtility::checkConfiguration()
{

  if (serviceName.length() != 0)
  {
    std::cout << "Checking MQ Connection for Service = " << serviceName << std::endl;
    Service *s{configuration->getServiceConfiguration(serviceName)};
    const std::string queueManager{s->getQueueManager()};
    std::cout << "MQ Queue Manager = " << queueManager << " for above service" << std::endl;
    MQClient client;
    if (client.initialise(queueManager)){
      MQConnection* mqc{client.connect(queueManager)};
      std::cout << "MQ_SUCCESS" << std::endl;
    }else{
      std::cout << "MQ_FAILURE" << std::endl;
    }
  }
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
