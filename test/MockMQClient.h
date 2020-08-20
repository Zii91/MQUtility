#pragma once
#include "gmock/gmock.h"
#include "src/lib/MQClient.h"
using pair_bool_int = std::pair<bool,int>;
using pair_bool_string = std::pair<bool,std::string>;
using MQConnection_ptr = std::shared_ptr<MQConnection>;

class MockMQClient: public MQClient {

  public:
    MOCK_METHOD(bool, initialise, (std::string const& queueManager),(override));
    MOCK_METHOD(MQConnection_ptr, connect, (std::string const& queueManager),(override));
    MOCK_METHOD(bool, openQueueConnection, (MQConnection_ptr const& mqConnection,std::string const& queue),(override));
    MOCK_METHOD(pair_bool_string, readQueue, (MQConnection_ptr const& mqConnection,std::string const& queue),(override));
    MOCK_METHOD(pair_bool_int, getQueueDepth, (MQConnection_ptr const& mqConnection,std::string const& queue),(override));
    MOCK_METHOD(bool, commit, (MQConnection_ptr const& mqConnection,std::string const& queue),(override));
    MOCK_METHOD(bool, rollback, (MQConnection_ptr const& mqConnection,std::string const& queue),(override));
    MOCK_METHOD(bool, closeQueueConnection, (MQConnection_ptr const& mqConnection,std::string const& queue),(override));
    MOCK_METHOD(bool, closeManagerConnection, (MQConnection_ptr const& mqConnection),(override));
};
