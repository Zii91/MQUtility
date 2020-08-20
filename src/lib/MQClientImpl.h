#pragma once
#include "MQClient.h"

class MQClientImpl : public MQClient
{
public:
  MQClientImpl(){};
  bool initialise(std::string const &queueManager) override
  {
    return true;
  };
  MQConnection_ptr connect(std::string const &queueManager) override
  {
    MQConnection_ptr mqc = make_shared<MQConnection>();
    mqc->setReturnCode(true);
    return mqc;
  };

  bool openQueueConnection(MQConnection_ptr const &mqConnection, std::string const &queue) override
  {
    return true;
  };
  pair_bool_string readQueue(MQConnection_ptr const &mqConnection, std::string const &queue) override
  {

    return std::make_pair(true, "Message");
  };
  pair_bool_int getQueueDepth(MQConnection_ptr const &mqConnection, std::string const &queue) override
  {
    return std::make_pair(true, 50);
  };
  bool commit(MQConnection_ptr const &mqConnection, std::string const &queue) override
  {
    return true;
  };
  bool rollback(MQConnection_ptr const &mqConnection, std::string const &queue) override
  {
    return true;
  };
  bool closeQueueConnection(MQConnection_ptr const &mqConnection, std::string const &queue) override
  {
    return true;
  };
  bool closeManagerConnection(MQConnection_ptr const &mqConnection) override
  {
    return true;
  };
};
