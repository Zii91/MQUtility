#pragma once
#include <string>

class MQConnection{
  bool returnCode;
  public:
    MQConnection();
    virtual bool getReturnCode() const;
    void setReturnCode(bool const& rc);
};
