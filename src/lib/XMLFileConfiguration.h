#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "Service.h"
#include "ConfigurationSource.h"
#include "tinyxml2.h"
#include "glog/logging.h"

using namespace std;


class XMLFileConfiguration: public ConfigurationSource
{
  string configFile;
  Service* parseServiceElement(tinyxml2::XMLElement &pElement);
  vector<Service*> serviceConfigurationVector;
public:
  XMLFileConfiguration(string const& fileName);
  void addService(Service* const service) override;
  void loadConfiguration() override;
  Service* getServiceConfiguration(string const& serviceName) override;
  vector<Service*> getServiceConfigurationVector() override;
};
