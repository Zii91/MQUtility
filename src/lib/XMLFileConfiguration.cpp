#include "XMLFileConfiguration.h"

using namespace std;

static const char *XPATH_SERVICE = "service";
static const char *XPATH_SERVICE_NAME = "serviceName";
static const char *XPATH_QUEUEMANAGER = "queueManager";
static const char *XPATH_QUEUES = "queues";
static const char *XPATH_QUEUE = "queue";
static const char *XPATH_LOGICAL_NAME = "logicalName";
static const char *XPATH_PHYSICAL_NAME = "physicalName";

XMLFileConfiguration::~XMLFileConfiguration()
{
  serviceConfigurationVector.clear();
}

XMLFileConfiguration::XMLFileConfiguration(string const &fileName)
{
  configFile = fileName;
}

void XMLFileConfiguration::loadConfiguration()
{
  LOG(INFO) << "Loading Configuration from XML file : " << configFile;
  tinyxml2::XMLDocument xmlDoc;
  tinyxml2::XMLError err = xmlDoc.LoadFile(configFile.c_str());

  if (err == tinyxml2::XML_ERROR_FILE_NOT_FOUND)
  {
    throw string("ERROR: File not found");
  }
  else if (err == tinyxml2::XML_ERROR_FILE_READ_ERROR)
  {
    throw string("ERROR: Read file error");
  }
  else if (err == tinyxml2::XML_ERROR_EMPTY_DOCUMENT)
  {
    throw string("ERROR: Document empty");
  }

  tinyxml2::XMLNode *pRoot = xmlDoc.FirstChild();
  if (pRoot == nullptr)
  {
    LOG(ERROR) << tinyxml2::XML_ERROR_FILE_READ_ERROR;
  }

  tinyxml2::XMLElement *pListService = pRoot->FirstChildElement(XPATH_SERVICE);
  if (pListService == nullptr)
  {
    throw string("ERROR: Not able to locate Service configuration");
  }
  while (pListService != nullptr)
  {
    //Parse service node and create new Service object
    shared_ptr<Service> sc{parseServiceElement(*pListService)};
    //Store it into serviceConfigurationVector
    serviceConfigurationVector.emplace_back(sc);
    //Move to the next node
    pListService = pListService->NextSiblingElement(XPATH_SERVICE);
  }
}

void XMLFileConfiguration::addService(shared_ptr<Service> const service)
{

  serviceConfigurationVector.emplace_back(service);
}

shared_ptr<Service> XMLFileConfiguration::parseServiceElement(tinyxml2::XMLElement &pElement)
{
  tinyxml2::XMLElement *pServiceNameElement{pElement.FirstChildElement(XPATH_SERVICE_NAME)};
  tinyxml2::XMLElement *pQueueManagerElement{pElement.FirstChildElement(XPATH_QUEUEMANAGER)};
  tinyxml2::XMLElement *pQueuesElement{pElement.FirstChildElement(XPATH_QUEUES)};
  if (pServiceNameElement == nullptr || pQueueManagerElement == nullptr || pQueuesElement == nullptr)
  {
    throw string("ERROR: Some of the mandatory nodes in configuration file are missing");
  }
  string serviceName = pServiceNameElement->GetText();
  string queueManager = pQueueManagerElement->GetText();
  shared_ptr<Service> sc = make_shared<Service>(serviceName, queueManager);

  tinyxml2::XMLElement *queueListElement = pQueuesElement->FirstChildElement(XPATH_QUEUE);
  while (queueListElement != nullptr)
  {
    string queueLogicalName = queueListElement->FirstChildElement(XPATH_LOGICAL_NAME)->GetText();
    string queuePhysicalName = queueListElement->FirstChildElement(XPATH_PHYSICAL_NAME)->GetText();
    sc->addQueue(queueLogicalName, queuePhysicalName);
    queueListElement = queueListElement->NextSiblingElement(XPATH_QUEUE);
  }
  return sc;
}

shared_ptr<Service> XMLFileConfiguration::getServiceConfiguration(string const &serviceName)
{
  auto const it = find_if(serviceConfigurationVector.begin(),
                          serviceConfigurationVector.end(), [serviceName](shared_ptr<Service> const obj) { return obj->getServiceName() == serviceName; });
  if (it != serviceConfigurationVector.end())
  {
    return *it;
  }
  return nullptr;
}

vector<shared_ptr<Service>> XMLFileConfiguration::getServiceConfigurationVector()
{
  return serviceConfigurationVector;
}
