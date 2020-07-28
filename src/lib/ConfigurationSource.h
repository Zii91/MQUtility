#include "Service.h"
#include <string>
using namespace std;
class ConfigurationSource
{
  public:
  virtual void loadConfiguration() = 0;
  virtual Service* getServiceConfiguration(string const& serviceName) = 0;
  virtual vector<Service*> getServiceConfigurationVector() = 0;
};
