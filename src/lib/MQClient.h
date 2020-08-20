#pragma once
#include "MQConnection.h"
#include <string>
#include <iostream>
using pair_bool_int = std::pair<bool,int>;
using pair_bool_string = std::pair<bool,std::string>;
using MQConnection_ptr = std::shared_ptr<MQConnection>;

class MQClient {

  public:
    virtual bool initialise(std::string const& queueManager)=0;
    virtual MQConnection_ptr connect(std::string const& queueManager)=0;
    virtual bool openQueueConnection(MQConnection_ptr const& mqConnection,std::string const& queue)=0;
    virtual pair_bool_string readQueue(MQConnection_ptr const& mqConnection,std::string const& queue)=0;
    virtual pair_bool_int getQueueDepth(MQConnection_ptr const& mqConnection,std::string const& queue)=0;
    virtual bool commit(MQConnection_ptr const& mqConnection,std::string const& queue)=0;
    virtual bool rollback(MQConnection_ptr const& mqConnection,std::string const& queue)=0;
    virtual bool closeQueueConnection(MQConnection_ptr const& mqConnection,std::string const& queue)=0;
    virtual bool closeManagerConnection(MQConnection_ptr const& mqConnection)=0;
    virtual ~MQClient(){};
};
