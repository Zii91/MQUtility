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
  MQConnection *connect(std::string const &queueManager) override
  {
    MQConnection *mqc = new MQConnection();
    mqc->setReturnCode(true);
    return mqc;
  };

  bool openQueueConnection(MQConnection* const &mqConnection, std::string const &queue) override
  {
    return true;
  };
  pair_bool_string readQueue(MQConnection* const &mqConnection, std::string const &queue) override
  {

    return std::make_pair(true, "Message");
  };
  pair_bool_int getQueueDepth(MQConnection* const &mqConnection, std::string const &queue) override
  {
    return std::make_pair(true, 50);
  };
  bool commit(MQConnection* const &mqConnection, std::string const &queue) override
  {
    return true;
  };
  bool rollback(MQConnection* const &mqConnection, std::string const &queue) override
  {
    return true;
  };
  bool closeQueueConnection(MQConnection* const &mqConnection, std::string const &queue) override
  {
    return true;
  };
  bool closeManagerConnection(MQConnection* const &mqConnection) override
  {
    return true;
  };
};
