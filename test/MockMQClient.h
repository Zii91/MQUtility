#pragma once
#include "gmock/gmock.h"
#include "src/lib/MQClient.h"
using pair_bool_int = std::pair<bool,int>;
using pair_bool_string = std::pair<bool,std::string>;

class MockMQClient: public MQClient {

  public:
    MOCK_METHOD(bool, initialise, (std::string const& queueManager),(override));
    MOCK_METHOD(MQConnection*, connect, (std::string const& queueManager),(override));
    MOCK_METHOD(bool, openQueueConnection, (MQConnection *const& mqConnection,std::string const& queue),(override));
    MOCK_METHOD(pair_bool_string, readQueue, (MQConnection *const& mqConnection,std::string const& queue),(override));
    MOCK_METHOD(pair_bool_int, getQueueDepth, (MQConnection *const& mqConnection,std::string const& queue),(override));
    MOCK_METHOD(bool, commit, (MQConnection *const& mqConnection,std::string const& queue),(override));
    MOCK_METHOD(bool, rollback, (MQConnection *const& mqConnection,std::string const& queue),(override));
    MOCK_METHOD(bool, closeQueueConnection, (MQConnection *const& mqConnection,std::string const& queue),(override));
    MOCK_METHOD(bool, closeManagerConnection, (MQConnection *const& mqConnection),(override));
};
