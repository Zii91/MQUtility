#pragma once
#include "MQConnection.h"
#include <string>
#include <iostream>
using pair_bool_int = std::pair<bool,int>;
using pair_bool_string = std::pair<bool,std::string>;

class MQClient {

  public:
    virtual bool initialise(std::string const& queueManager)=0;
    virtual MQConnection* connect(std::string const& queueManager)=0;
    virtual bool openQueueConnection(MQConnection *const& mqConnection,std::string const& queue)=0;
    virtual pair_bool_string readQueue(MQConnection *const& mqConnection,std::string const& queue)=0;
    virtual pair_bool_int getQueueDepth(MQConnection *const& mqConnection,std::string const& queue)=0;
    virtual bool commit(MQConnection *const& mqConnection,std::string const& queue)=0;
    virtual bool rollback(MQConnection *const& mqConnection,std::string const& queue)=0;
    virtual bool closeQueueConnection(MQConnection *const& mqConnection,std::string const& queue)=0;
    virtual bool closeManagerConnection(MQConnection *const& mqConnection)=0;
};
