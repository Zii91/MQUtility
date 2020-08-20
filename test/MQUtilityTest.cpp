#include "src/lib/MQUtility.h"
#include "MockMQClient.h"
#include <string>
#include <stdlib.h>
#include <time.h>
#include "gtest/gtest.h"

//Usage of fixture to refactor Arrangement part and memory management
class XMLFileConfigurationInitializationShould : public ::testing::Test
{
protected:
  void SetUp(string const &fileName)
  {
    config = std::unique_ptr<XMLFileConfiguration>(new XMLFileConfiguration(fileName));
  }

  virtual void TearDown()
  {

  }
  unique_ptr<ConfigurationSource> config;
};

TEST(XMLFileConfigurationShould, ReturnSameService)
{
  unique_ptr<ConfigurationSource> config = std::unique_ptr<XMLFileConfiguration>(new XMLFileConfiguration(""));
  shared_ptr<Service> expected = make_shared<Service>("Service3","DefaultConfiguration");
  config->addService(expected);
  shared_ptr<Service> actual = config->getServiceConfiguration(expected->getServiceName());
  EXPECT_EQ(expected,actual);
};

TEST_F(XMLFileConfigurationInitializationShould, ReturnSameQueueNumberForAServiceFromXMLFile)
{
  SetUp("/Users/Zied/Projects/STAR-MQ-Utility/configuration.xml");
  config->loadConfiguration();
  int expected = 2;
  int actual = config->getServiceConfiguration("Service1")->getQueueMap().size();
  EXPECT_EQ(expected, actual);
};

TEST_F(XMLFileConfigurationInitializationShould, ReturnSameServiceNumberFromXMLFile)
{
  SetUp("/Users/Zied/Projects/STAR-MQ-Utility/configuration.xml");
  config->loadConfiguration();
  int expected = 2;
  int actual = config->getServiceConfigurationVector().size();
  EXPECT_EQ(expected, actual);
};

TEST_F(XMLFileConfigurationInitializationShould, CatchExceptionXMLFileNotFound)
{
  SetUp("/Users/Zied/Projects/STAR-MQ-Utility/nonExistentFile.xml");
  EXPECT_THROW({
    try
    {
      config->loadConfiguration();
    }
    catch (std::string const &e)
    {
      EXPECT_EQ("ERROR: File not found", e);
      throw;
    }
  },
               std::string);
};

TEST_F(XMLFileConfigurationInitializationShould, CatchExceptionXMLFileEmpty)
{
  SetUp("/Users/Zied/Projects/STAR-MQ-Utility/emptyFile.xml");
  EXPECT_THROW({
    try
    {
      config->loadConfiguration();
    }
    catch (std::string const &e)
    {
      EXPECT_EQ("ERROR: Document empty", e);
      throw;
    }
  },
               std::string);
};

TEST_F(XMLFileConfigurationInitializationShould, CatchExceptionXMLFileCorrupted)
{
  SetUp("/Users/Zied/Projects/STAR-MQ-Utility/configurationCorrupted.xml");
  EXPECT_THROW({
    try
    {
      config->loadConfiguration();
    }
    catch (std::string const &e)
    {
      EXPECT_EQ("ERROR: Some of the mandatory nodes in configuration file are missing", e);
      throw;
    }
  },
               std::string);
};

//Usage of fixture to refactor Arrangement part and memory management
class MQUtilityCheckingShould : public ::testing::Test
{
protected:
  void SetUp()
  {
    util = make_shared<MQUtility::MQUtility>("/Users/Zied/Projects/STAR-MQ-Utility/configuration.xml", "Service1", MQUtility::XML_MODE);
  }

  virtual void TearDown()
  {

  }
  shared_ptr<MQUtility::MQUtility> util;
};

TEST_F(MQUtilityCheckingShould, returnSuccessCodeWhenCanAccessAllConfigurationQueuesForOneService)
{
  shared_ptr<testing::NiceMock<MockMQClient>> client = make_shared<testing::NiceMock<MockMQClient>>();
  srand(time(NULL));
  MQConnection_ptr connection = make_shared<MQConnection>();
  ON_CALL(*client, initialise(testing::_)).WillByDefault(testing::Return(true));
  ON_CALL(*client, connect(testing::_)).WillByDefault(testing::Return(connection));
  ON_CALL(*client, openQueueConnection(connection, testing::_)).WillByDefault(testing::Return(true));
  ON_CALL(*client, getQueueDepth(connection, testing::_)).WillByDefault(testing::Return(std::make_pair(true, (rand() % 100))));
  util->init();
  util->setMQClient(client);
  int rc = util->checkConfiguration();
  EXPECT_EQ(rc, 1);
};


TEST_F(MQUtilityCheckingShould, returnErrorCodeWhenCannotInitialiseClient)
{
  shared_ptr<testing::NiceMock<MockMQClient>> client = make_shared<testing::NiceMock<MockMQClient>>();
  ON_CALL(*client, initialise(testing::_)).WillByDefault(testing::Return(false));
  util->init();
  util->setMQClient(client);
  int rc = util->checkConfiguration();
  EXPECT_EQ(rc, -1);
};

TEST_F(MQUtilityCheckingShould, returnErrorCodeWhenCannotConnectClient)
{
  shared_ptr<testing::NiceMock<MockMQClient>> client = make_shared<testing::NiceMock<MockMQClient>>();
  MQConnection_ptr connection = make_shared<MQConnection>();
  connection->setReturnCode(false);
  ON_CALL(*client, initialise(testing::_)).WillByDefault(testing::Return(true));
  ON_CALL(*client, connect(testing::_)).WillByDefault(testing::Return(connection));
  util->init();
  util->setMQClient(client);
  int rc = util->checkConfiguration();
  EXPECT_EQ(rc, -1);
};

TEST_F(MQUtilityCheckingShould, returnErrorCodeWhenCannotOpenConnectionToQueue)
{
  shared_ptr<testing::NiceMock<MockMQClient>> client = make_shared<testing::NiceMock<MockMQClient>>();
  MQConnection_ptr connection = make_shared<MQConnection>();
  ON_CALL(*client, initialise(testing::_)).WillByDefault(testing::Return(true));
  ON_CALL(*client, connect(testing::_)).WillByDefault(testing::Return(connection));
  ON_CALL(*client, openQueueConnection(connection, testing::_)).WillByDefault(testing::Return(false));
  util->init();
  util->setMQClient(client);
  int rc = util->checkConfiguration();
  EXPECT_EQ(rc, -1);
};

TEST_F(MQUtilityCheckingShould, returnErrorCodeWhenCannotOpenConnectionToOneOfTheQueues)
{
  shared_ptr<testing::NiceMock<MockMQClient>> client = make_shared<testing::NiceMock<MockMQClient>>();
  MQConnection_ptr connection = make_shared<MQConnection>();
  ON_CALL(*client, initialise(testing::_)).WillByDefault(testing::Return(true));
  ON_CALL(*client, connect(testing::_)).WillByDefault(testing::Return(connection));
  EXPECT_CALL(*client, openQueueConnection(connection, testing::_)).Times(2).WillOnce(testing::Return(false)).WillOnce(testing::Return(true));
  util->init();
  util->setMQClient(client);
  int rc = util->checkConfiguration();
  EXPECT_EQ(rc, -1);
};

TEST_F(MQUtilityCheckingShould, returnErrorCodeWhenCannotOpenConnectionToOneOfTheQueuesForOneService)
{
  shared_ptr<testing::NiceMock<MockMQClient>> client = make_shared<testing::NiceMock<MockMQClient>>();
  MQConnection_ptr connection = make_shared<MQConnection>();
  ON_CALL(*client, initialise(testing::_)).WillByDefault(testing::Return(true));
  ON_CALL(*client, connect(testing::_)).WillByDefault(testing::Return(connection));
  EXPECT_CALL(*client, openQueueConnection(connection, testing::_)).WillOnce(testing::Return(false)).WillRepeatedly(testing::Return(true));
  util->setServiceName("");
  util->init();
  util->setMQClient(client);
  int rc = util->checkConfiguration();
  EXPECT_EQ(rc, -1);
};

TEST_F(MQUtilityCheckingShould, returnErrorCodeWhenCannotOpenConnectionToAllQueues)
{
  shared_ptr<testing::NiceMock<MockMQClient>> client = make_shared<testing::NiceMock<MockMQClient>>();
  MQConnection_ptr connection = make_shared<MQConnection>();
  ON_CALL(*client, initialise(testing::_)).WillByDefault(testing::Return(true));
  ON_CALL(*client, connect(testing::_)).WillByDefault(testing::Return(connection));
  EXPECT_CALL(*client, openQueueConnection(connection, testing::_)).WillRepeatedly(testing::Return(false));
  util->setServiceName("");
  util->init();
  util->setMQClient(client);
  int rc = util->checkConfiguration();
  EXPECT_EQ(rc, -1);
};

TEST_F(MQUtilityCheckingShould, returnSuccessCodeWhenCanAccessAllConfigurationQueuesForAllServices)
{
  shared_ptr<testing::NiceMock<MockMQClient>> client = make_shared<testing::NiceMock<MockMQClient>>();
  srand(time(NULL));
  MQConnection_ptr connection = make_shared<MQConnection>();
  ON_CALL(*client, initialise(testing::_)).WillByDefault(testing::Return(true));
  ON_CALL(*client, connect(testing::_)).WillByDefault(testing::Return(connection));
  ON_CALL(*client, openQueueConnection(connection, testing::_)).WillByDefault(testing::Return(true));
  ON_CALL(*client, getQueueDepth(connection, testing::_)).WillByDefault(testing::Return(std::make_pair(true, (rand() % 100))));
  util->setServiceName("");
  util->init();
  util->setMQClient(client);
  int rc = util->checkConfiguration();
  EXPECT_EQ(rc, 1);
};

TEST_F(MQUtilityCheckingShould, returnErrorCodeWhenMQClientIsNull)
{
  util->setServiceName("");
  util->init();
  int rc = util->checkConfiguration();
  EXPECT_EQ(rc, -1);
};

