#pragma once
#include <string>

class MQConnection{
  bool returnCode=true;
  public:
    MQConnection();
    virtual bool getReturnCode() const;
    void setReturnCode(bool const& rc);
    virtual ~MQConnection(){};
};
