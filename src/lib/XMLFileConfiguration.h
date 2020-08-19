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
  vector<shared_ptr<Service>> serviceConfigurationVector;
  shared_ptr<Service> parseServiceElement(tinyxml2::XMLElement &pElement);

public:
  ~XMLFileConfiguration();
  XMLFileConfiguration(string const &fileName);
  void addService(shared_ptr<Service> const service) override;
  void loadConfiguration() override;
  shared_ptr<Service> getServiceConfiguration(string const &serviceName) override;
  vector<shared_ptr<Service>> getServiceConfigurationVector() override;
};
