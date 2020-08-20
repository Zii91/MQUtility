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
      void initWithAPISource();
    public:
      MQUtility(std::string const &filename, int const& mode); //Constructor of tool used when no service name is defined
      MQUtility(std::string const &filename, string const& serviceName, int const& mode); //Constructor of tool used when a service name is defined
      shared_ptr<MQClient> getMQClient() const;
      void setMQClient(shared_ptr<MQClient> const& mqClient); //Setter of MQClient (used by UnitTests to set MockMQClient)
      void setServiceName(string const& service);
      void init();  // Initialisation of configuration depending on the source
      int  checkConfiguration(); // Checking the consistency of MQ Connectivity for loaded configuration
      void printConfigurationReport(std::string const& service, int const& status) const; // Print the configuration and the result of check task.
      virtual ~MQUtility(){};
  };
}
