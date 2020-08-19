#include "src/lib/MQUtility.h"
#include "glog/logging.h"
int main(int argc, char *argv[])
{
  FLAGS_log_dir ="logs";
  google::InitGoogleLogging(argv[0]);
  if (argc < 2)
  {
    try
    {
      throw std::string("Argument 'File name' is mandatory to run the program");
    }
    catch (std::string const &e)
    {
      LOG(ERROR) << e ;
      return 1;
    }
  }

  MQUtility::MQUtility* mqUtility;
  if (argc == 2)
  {
    mqUtility  = new MQUtility::MQUtility(argv[1],MQUtility::XML_MODE);
  }else{
    mqUtility = new MQUtility::MQUtility(argv[1],argv[2],MQUtility::XML_MODE);
  }

  mqUtility->init();
  // TODO set MQClientImpl to mqUtility
  //mqUtility->setMQClient(new MQClientImpl());
  mqUtility->checkConfiguration();
}
