#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "Service.h"
#include "IConfigurationSource.h"
#include "tinyxml2.h"
using namespace std;


class XMLFileConfiguration: public IConfigurationSource
{
  string configFile;
  Service* parseServiceElement(tinyxml2::XMLElement &pElement);
  vector<Service*> serviceConfigurationVector;
public:
  XMLFileConfiguration(string const& fileName);
  void loadConfiguration() override;
  Service* getServiceConfiguration(string const& serviceName) override;
  vector<Service*> getServiceConfigurationVector() override;
};