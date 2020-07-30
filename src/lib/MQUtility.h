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

  enum MQ_RETURN_CODE {
    MQ_FAILURE = -1,
    MQ_SUCCESS = 1,
  };

  class MQUtility {

      std::string configurationFile;
      std::string serviceName;
      MQClient* mqClient;

      ConfigurationSource* configuration;
      int mode;
      int  checkConfiguration(std::string const& serviceName);
      void initWithXMLSource();
      void initWithDBSource();
      void initAPISource();

    public:
      MQUtility(std::string const &filename, int const& mode);
      MQUtility(std::string const &filename, string const& serviceName, int const& mode);
      MQClient* getMQClient() const;
      void setMQClient(MQClient* const& mqClient);
      void init();
      int  checkConfiguration();
      void printConfigurationReport(std::string const& service, int const& status) const;
  };
}
