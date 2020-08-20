#include "Service.h"
#include <string>
#include <memory>
using namespace std;

//Creation of Interface ConfigurationSource as sources are keen to be upgraded
// XMLFile, Database, API ...
class ConfigurationSource
{
  public:
  virtual ~ConfigurationSource(){};
  virtual void loadConfiguration() = 0;
  virtual void addService(shared_ptr<Service> const service) = 0 ;
  virtual shared_ptr<Service> getServiceConfiguration(string const& serviceName) = 0;
  virtual vector<shared_ptr<Service>> getServiceConfigurationVector() = 0;
};
