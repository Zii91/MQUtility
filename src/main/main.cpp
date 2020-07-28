#include "src/lib/MQUtility.h"

int main(int argc, char *argv[])
{
  if (argc < 2)
  {
    try
    {
      throw std::string("Argument 'File name' is mandatory to run the program");
    }
    catch (std::string const &e)
    {
      cerr << e << endl;
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

  mqUtility->run();
}
