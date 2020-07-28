#include "MQConnection.h"


    MQConnection::MQConnection(){};
    bool MQConnection::getReturnCode() const{
      return returnCode;
    };
    void MQConnection::setReturnCode(bool const& rc){
      returnCode = rc;
    };
