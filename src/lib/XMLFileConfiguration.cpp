#include "XMLFileConfiguration.h"

using namespace std;

XMLFileConfiguration::XMLFileConfiguration(string const &fileName)
{
  configFile = fileName;
}

void XMLFileConfiguration::loadConfiguration()
{
  cout << "Loading Configuration from XML file : " << configFile << endl;
  tinyxml2::XMLDocument xmlDoc;
  tinyxml2::XMLError err = xmlDoc.LoadFile(configFile.c_str());

  if (err == tinyxml2::XML_ERROR_FILE_NOT_FOUND){
      throw string("ERROR: File not found");
  }else if (err == tinyxml2::XML_ERROR_FILE_READ_ERROR){
      throw string("ERROR: Read file error");
  }else if (err == tinyxml2::XML_ERROR_EMPTY_DOCUMENT){
      throw string("ERROR: Document empty");
  }

  tinyxml2::XMLNode *pRoot = xmlDoc.FirstChild();
  if (pRoot == nullptr)
  {
    cout << tinyxml2::XML_ERROR_FILE_READ_ERROR << endl;
  }

  tinyxml2::XMLElement *pListService = pRoot->FirstChildElement("service");
  if (pListService == nullptr){
    throw string("ERROR: Not able to locate Service configuration");
  }
  while (pListService != nullptr)
  {
    //Parse service node and create new Service object
    Service* sc{parseServiceElement(*pListService)};
    //Store it into serviceConfigurationVector
    serviceConfigurationVector.emplace_back(sc);
    //Move to the next node
    pListService = pListService->NextSiblingElement("service");
  }
}

Service* XMLFileConfiguration::parseServiceElement(tinyxml2::XMLElement &pElement)
{
  tinyxml2::XMLElement* pServiceNameElement{pElement.FirstChildElement("serviceName")};
  tinyxml2::XMLElement* pQueueManagerElement{pElement.FirstChildElement("queueManager")};
  tinyxml2::XMLElement* pQueuesElement{pElement.FirstChildElement("queues")};
  if (pServiceNameElement == nullptr || pQueueManagerElement == nullptr || pQueuesElement == nullptr){
    throw string("ERROR: Some of the mandatory nodes in configuration file are missing");
  }
  string serviceName = pServiceNameElement->GetText();
  string queueManager = pQueueManagerElement->GetText();
  Service* sc = new Service(serviceName, queueManager);

  tinyxml2::XMLElement *queueListElement = pQueuesElement->FirstChildElement("queue");
  while (queueListElement != nullptr)
  {
    string queueLogicalName = queueListElement->FirstChildElement("logicalName")->GetText();
    string queuePhysicalName = queueListElement->FirstChildElement("physicalName")->GetText();
    sc->addQueue(queueLogicalName, queuePhysicalName);
    queueListElement = queueListElement->NextSiblingElement("queue");
  }
  return std::move(sc);
}

Service* XMLFileConfiguration::getServiceConfiguration(string const& serviceName){
  auto const it = find_if(serviceConfigurationVector.begin(),
    serviceConfigurationVector.end(), [serviceName](Service* const  obj) {return obj->getServiceName()==serviceName;});
  if (it != serviceConfigurationVector.end())
  {
    return *it;
  }
  return nullptr;
}

vector<Service*> XMLFileConfiguration::getServiceConfigurationVector(){
  return serviceConfigurationVector;
}
