#pragma once
#include "XMLFileConfiguration.h"
#include "MQClient.h"
#include <iostream>
#include <vector>
#include <string>
namespace MQUtility
{
  enum MQUtilityMode {
    XML_MODE = 0,
    DB_MODE = 1,
    REST_API_MODE = 2
  };

  class MQUtility {

      std::string configurationFile;
      std::string serviceName;
      MQClient* mqClient;

      ConfigurationSource* configuration;
      int mode;
      void init();
      int checkConfiguration();
      void initWithXMLSource();
      void initWithDBSource();
      void initAPISource();

    public:
      MQUtility(std::string const &filename, int const& mode);
      MQUtility(std::string const &filename, string const& serviceName, int const& mode);
      MQClient* getMQClient();
      void setMQClient(MQClient* const& mqClient);
      int run();
      void printConfiguration();
  };
}
