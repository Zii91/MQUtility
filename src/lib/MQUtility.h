#pragma once
#include "XMLFileConfiguration.h"
#include "MQClient.h"
#include "MQClientImpl.h"
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
      shared_ptr<MQClient> mqClient;

      unique_ptr<ConfigurationSource> configuration;
      int mode;
      int  checkConfiguration(std::string const& serviceName);
      void initWithXMLSource();
      void initWithDBSource();
      void initAPISource();

    public:
      MQUtility(std::string const &filename, int const& mode);
      MQUtility(std::string const &filename, string const& serviceName, int const& mode);
      shared_ptr<MQClient> getMQClient() const;
      void setMQClient(shared_ptr<MQClient> const& mqClient);
      void setServiceName(string const& service);
      void init();
      int  checkConfiguration();
      void printConfigurationReport(std::string const& service, int const& status) const;

  };
}
