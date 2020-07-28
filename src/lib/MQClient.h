#include "gmock/gmock.h"
#include "MQConnection.h"
#include <string>
#include <iostream>
using pair_bool_int = std::pair<bool,int>;
using pair_bool_string = std::pair<bool,std::string>;

class MQClient {

  public:
    MOCK_METHOD(bool, initialise, (std::string const& queueManager));
    MOCK_METHOD(MQConnection*, connect, (std::string const& queueManager));
    MOCK_METHOD(std::string, openQueueConnection, (MQConnection const& mqConnection,std::string const& queue));
    MOCK_METHOD(pair_bool_string, readQueue, (MQConnection const& mqConnection,std::string const& queue));
    MOCK_METHOD(pair_bool_int, getQueueDepth, (MQConnection const& mqConnection,std::string const& queue));
    MOCK_METHOD(bool, commit, (MQConnection const& mqConnection,std::string const& queue));
    MOCK_METHOD(bool, rollback, (MQConnection const& mqConnection,std::string const& queue));
    MOCK_METHOD(bool, closeQueueConnection, (MQConnection const& mqConnection,std::string const& queue));
    MOCK_METHOD(bool, closeManagerConnection, (MQConnection const& mqConnection));
};
