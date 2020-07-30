#include "src/lib/MQUtility.h"
#include "MockMQClient.h"
#include <string>
#include <stdlib.h>
#include <time.h>
#include "gtest/gtest.h"

TEST(MQUtilityInitializationShould, ReturnSameServiceNameFromXMLFile){
  XMLFileConfiguration *cfg = new XMLFileConfiguration("/Users/Zied/Projects/STAR-MQ-Utility/configuration.xml");
  cfg->loadConfiguration();
  std::string expected = "Service1";
  std::string actual = cfg->getServiceConfiguration(expected)->getServiceName();
  EXPECT_EQ(expected,actual);
};

TEST(MQUtilityInitializationShould, ReturnSameServiceNumberFromXMLFile){
  XMLFileConfiguration *cfg = new XMLFileConfiguration("/Users/Zied/Projects/STAR-MQ-Utility/configuration.xml");
  cfg->loadConfiguration();
  int expected = 2;
  int actual = cfg->getServiceConfigurationVector().size();
  EXPECT_EQ(expected,actual);
}

TEST(MQUtilityInitializationShould, ReturnSameQueueNumberForAServiceFromXMLFile){
  XMLFileConfiguration *cfg = new XMLFileConfiguration("/Users/Zied/Projects/STAR-MQ-Utility/configuration.xml");
  cfg->loadConfiguration();
  int expected = 2;
  int actual = cfg->getServiceConfiguration("Service1")->getQueueMap().size();
  EXPECT_EQ(expected,actual);
}

TEST(MQUtilityInitializationShould, CatchExceptionXMLFileNotFound){
  XMLFileConfiguration *cfg = new XMLFileConfiguration("/Users/Zied/Projects/STAR-MQ-Utility/nonExistentFile.xml");
  EXPECT_THROW({
        try
        {
            cfg->loadConfiguration();
        }
        catch(std::string const& e )
        {
            // and this tests that it has the correct message
            EXPECT_EQ( "ERROR: File not found", e );
            throw;
        }
    }, std::string );
}

TEST(MQUtilityInitializationShould, CatchExceptionXMLFileEmpty){
  XMLFileConfiguration *cfg = new XMLFileConfiguration("/Users/Zied/Projects/STAR-MQ-Utility/emptyFile.xml");
  EXPECT_THROW({
        try
        {
            cfg->loadConfiguration();
        }
        catch(std::string const& e )
        {
            // and this tests that it has the correct message
            EXPECT_EQ( "ERROR: Document empty", e );
            throw;
        }
    }, std::string );
}

TEST(MQUtilityInitializationShould, CatchExceptionXMLFileCorrupted){
  XMLFileConfiguration *cfg = new XMLFileConfiguration("/Users/Zied/Projects/STAR-MQ-Utility/configurationCorrupted.xml");
  EXPECT_THROW({
        try
        {
            cfg->loadConfiguration();
        }
        catch(std::string const& e )
        {
            // and this tests that it has the correct message
            EXPECT_EQ( "ERROR: Some of the mandatory nodes in configuration file are missing", e );
            throw;
        }
    }, std::string );
}



TEST(MQUtilityCheckingShould, accessAllConfigurationQueuesForOneService){
  testing::NiceMock<MockMQClient> client;
  srand(time(NULL));
  MQConnection* connection = new MQConnection();
  ON_CALL(client, initialise(testing::_)).WillByDefault(testing::Return(true));
  ON_CALL(client, connect(testing::_)).WillByDefault(testing::Return(connection));
  ON_CALL(client, openQueueConnection(connection,testing::_)).WillByDefault(testing::Return(true));
  ON_CALL(client, getQueueDepth(connection,testing::_)).WillByDefault(testing::Return(std::make_pair(true,(rand() % 100))));
  MQUtility::MQUtility* util = new MQUtility::MQUtility("/Users/Zied/Projects/STAR-MQ-Utility/configuration.xml","Service1",MQUtility::XML_MODE);
  util->init();
  util->setMQClient(&client);
  int rc = util->checkConfiguration();
  EXPECT_EQ(rc,1);
};

TEST(MQUtilityCheckingShould, accessAllConfigurationQueuesForAllServices){
  testing::NiceMock<MockMQClient> client;
  srand(time(NULL));
  MQConnection* connection = new MQConnection();
  ON_CALL(client, initialise(testing::_)).WillByDefault(testing::Return(true));
  ON_CALL(client, connect(testing::_)).WillByDefault(testing::Return(connection));
  ON_CALL(client, openQueueConnection(connection,testing::_)).WillByDefault(testing::Return(true));
  ON_CALL(client, getQueueDepth(connection,testing::_)).WillByDefault(testing::Return(std::make_pair(true,(rand() % 100))));
  MQUtility::MQUtility* util = new MQUtility::MQUtility("/Users/Zied/Projects/STAR-MQ-Utility/configuration.xml",MQUtility::XML_MODE);
  util->init();
  util->setMQClient(&client);
  int rc = util->checkConfiguration();
  EXPECT_EQ(rc,1);
};

TEST(MQUtilityCheckingShould, cannotInitialiseClient){
  testing::NiceMock<MockMQClient> client;
  ON_CALL(client, initialise(testing::_)).WillByDefault(testing::Return(false));
  MQUtility::MQUtility* util = new MQUtility::MQUtility("/Users/Zied/Projects/STAR-MQ-Utility/configuration.xml","Service1",MQUtility::XML_MODE);
  util->init();
  util->setMQClient(&client);
  int rc = util->checkConfiguration();
  EXPECT_EQ(rc,-1);
};

TEST(MQUtilityCheckingShould, cannotConnectClient){
  testing::NiceMock<MockMQClient> client;
  MQConnection* connection = new MQConnection();
  connection->setReturnCode(false);
  ON_CALL(client, initialise(testing::_)).WillByDefault(testing::Return(true));
  ON_CALL(client, connect(testing::_)).WillByDefault(testing::Return(connection));
  MQUtility::MQUtility* util = new MQUtility::MQUtility("/Users/Zied/Projects/STAR-MQ-Utility/configuration.xml","Service1",MQUtility::XML_MODE);
  util->init();
  util->setMQClient(&client);
  int rc = util->checkConfiguration();
  EXPECT_EQ(rc,-1);
};

TEST(MQUtilityCheckingShould, cannotOpenConnectionToQueue){
  testing::NiceMock<MockMQClient> client;
  MQConnection* connection = new MQConnection();
  ON_CALL(client, initialise(testing::_)).WillByDefault(testing::Return(true));
  ON_CALL(client, connect(testing::_)).WillByDefault(testing::Return(connection));
  ON_CALL(client, openQueueConnection(connection,testing::_)).WillByDefault(testing::Return(false));
  MQUtility::MQUtility* util = new MQUtility::MQUtility("/Users/Zied/Projects/STAR-MQ-Utility/configuration.xml","Service1",MQUtility::XML_MODE);
  util->init();
  util->setMQClient(&client);
  int rc = util->checkConfiguration();
  EXPECT_EQ(rc,-1);
};

TEST(MQUtilityCheckingShould, cannotOpenConnectionToOneOfTheQueues){
  testing::NiceMock<MockMQClient> client;
  MQConnection* connection = new MQConnection();
  ON_CALL(client, initialise(testing::_)).WillByDefault(testing::Return(true));
  ON_CALL(client, connect(testing::_)).WillByDefault(testing::Return(connection));
  EXPECT_CALL(client, openQueueConnection(connection,testing::_)).Times(2).WillOnce(testing::Return(false)).WillOnce(testing::Return(true));
  MQUtility::MQUtility* util = new MQUtility::MQUtility("/Users/Zied/Projects/STAR-MQ-Utility/configuration.xml","Service1",MQUtility::XML_MODE);
  util->init();
  util->setMQClient(&client);
  int rc = util->checkConfiguration();
  EXPECT_EQ(rc,-1);
};

TEST(MQUtilityCheckingShould, cannotOpenConnectionToOneOfTheQueues2){
  testing::NiceMock<MockMQClient> client;
  MQConnection* connection = new MQConnection();
  ON_CALL(client, initialise(testing::_)).WillByDefault(testing::Return(true));
  ON_CALL(client, connect(testing::_)).WillByDefault(testing::Return(connection));
  EXPECT_CALL(client, openQueueConnection(connection,testing::_)).WillOnce(testing::Return(false)).WillRepeatedly(testing::Return(true));
  MQUtility::MQUtility* util = new MQUtility::MQUtility("/Users/Zied/Projects/STAR-MQ-Utility/configuration.xml",MQUtility::XML_MODE);
  util->init();
  util->setMQClient(&client);
  int rc = util->checkConfiguration();
  EXPECT_EQ(rc,-1);
};
