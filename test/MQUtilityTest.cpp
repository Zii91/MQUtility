#include "src/lib/MQUtility.h"
#include "MockMQClient.h"
#include <string>
#include <stdlib.h>
#include <time.h>
#include "gtest/gtest.h"

class XMLFileConfigurationInitializationShould : public ::testing::Test
{
protected:
  void SetUp(string const &fileName)
  {
    config = new XMLFileConfiguration(fileName);
  }

  virtual void TearDown()
  {
    delete config;
    config = nullptr;
  }
  ConfigurationSource *config;
};

TEST(XMLFileConfigurationShould, ReturnSameService)
{
  ConfigurationSource* config = new XMLFileConfiguration("");
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

class MQUtilityCheckingShould : public ::testing::Test
{
protected:
  void SetUp()
  {
    util = new MQUtility::MQUtility("/Users/Zied/Projects/STAR-MQ-Utility/configuration.xml", "Service1", MQUtility::XML_MODE);
  }

  virtual void TearDown()
  {
    delete util;
    util = nullptr;
  }
  MQUtility::MQUtility *util;
};

TEST_F(MQUtilityCheckingShould, accessAllConfigurationQueuesForOneService)
{
  shared_ptr<testing::NiceMock<MockMQClient>> client = make_shared<testing::NiceMock<MockMQClient>>();
  srand(time(NULL));
  MQConnection *connection = new MQConnection();
  ON_CALL(*client, initialise(testing::_)).WillByDefault(testing::Return(true));
  ON_CALL(*client, connect(testing::_)).WillByDefault(testing::Return(connection));
  ON_CALL(*client, openQueueConnection(connection, testing::_)).WillByDefault(testing::Return(true));
  ON_CALL(*client, getQueueDepth(connection, testing::_)).WillByDefault(testing::Return(std::make_pair(true, (rand() % 100))));
  util->init();
  util->setMQClient(client);
  int rc = util->checkConfiguration();
  EXPECT_EQ(rc, 1);
};

TEST_F(MQUtilityCheckingShould, cannotInitialiseClient)
{
  shared_ptr<testing::NiceMock<MockMQClient>> client = make_shared<testing::NiceMock<MockMQClient>>();
  ON_CALL(*client, initialise(testing::_)).WillByDefault(testing::Return(false));
  util->init();
  util->setMQClient(client);
  int rc = util->checkConfiguration();
  EXPECT_EQ(rc, -1);
};

TEST_F(MQUtilityCheckingShould, cannotConnectClient)
{
  shared_ptr<testing::NiceMock<MockMQClient>> client = make_shared<testing::NiceMock<MockMQClient>>();
  MQConnection *connection = new MQConnection();
  connection->setReturnCode(false);
  ON_CALL(*client, initialise(testing::_)).WillByDefault(testing::Return(true));
  ON_CALL(*client, connect(testing::_)).WillByDefault(testing::Return(connection));
  util->init();
  util->setMQClient(client);
  int rc = util->checkConfiguration();
  EXPECT_EQ(rc, -1);
};

TEST_F(MQUtilityCheckingShould, cannotOpenConnectionToQueue)
{
  shared_ptr<testing::NiceMock<MockMQClient>> client = make_shared<testing::NiceMock<MockMQClient>>();
  MQConnection *connection = new MQConnection();
  ON_CALL(*client, initialise(testing::_)).WillByDefault(testing::Return(true));
  ON_CALL(*client, connect(testing::_)).WillByDefault(testing::Return(connection));
  ON_CALL(*client, openQueueConnection(connection, testing::_)).WillByDefault(testing::Return(false));
  util->init();
  util->setMQClient(client);
  int rc = util->checkConfiguration();
  EXPECT_EQ(rc, -1);
};

TEST_F(MQUtilityCheckingShould, cannotOpenConnectionToOneOfTheQueues)
{
  shared_ptr<testing::NiceMock<MockMQClient>> client = make_shared<testing::NiceMock<MockMQClient>>();
  MQConnection *connection = new MQConnection();
  ON_CALL(*client, initialise(testing::_)).WillByDefault(testing::Return(true));
  ON_CALL(*client, connect(testing::_)).WillByDefault(testing::Return(connection));
  EXPECT_CALL(*client, openQueueConnection(connection, testing::_)).Times(2).WillOnce(testing::Return(false)).WillOnce(testing::Return(true));
  util->init();
  util->setMQClient(client);
  int rc = util->checkConfiguration();
  EXPECT_EQ(rc, -1);
};

TEST_F(MQUtilityCheckingShould, cannotOpenConnectionToOneOfTheQueues2)
{
  shared_ptr<testing::NiceMock<MockMQClient>> client = make_shared<testing::NiceMock<MockMQClient>>();
  MQConnection *connection = new MQConnection();
  ON_CALL(*client, initialise(testing::_)).WillByDefault(testing::Return(true));
  ON_CALL(*client, connect(testing::_)).WillByDefault(testing::Return(connection));
  EXPECT_CALL(*client, openQueueConnection(connection, testing::_)).WillOnce(testing::Return(false)).WillRepeatedly(testing::Return(true));
  util->setServiceName("");
  util->init();
  util->setMQClient(client);
  int rc = util->checkConfiguration();
  EXPECT_EQ(rc, -1);
};

TEST_F(MQUtilityCheckingShould, cannotOpenConnectionToAllQueues)
{
  shared_ptr<testing::NiceMock<MockMQClient>> client = make_shared<testing::NiceMock<MockMQClient>>();
  MQConnection *connection = new MQConnection();
  ON_CALL(*client, initialise(testing::_)).WillByDefault(testing::Return(true));
  ON_CALL(*client, connect(testing::_)).WillByDefault(testing::Return(connection));
  EXPECT_CALL(*client, openQueueConnection(connection, testing::_)).WillRepeatedly(testing::Return(false));
  util->setServiceName("");
  util->init();
  util->setMQClient(client);
  int rc = util->checkConfiguration();
  EXPECT_EQ(rc, -1);
};

TEST_F(MQUtilityCheckingShould, accessAllConfigurationQueuesForAllServices)
{
  shared_ptr<testing::NiceMock<MockMQClient>> client = make_shared<testing::NiceMock<MockMQClient>>();
  srand(time(NULL));
  MQConnection *connection = new MQConnection();
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


