#include "src/lib/XMLFileConfiguration.h"
#include <string>
#include <iostream>
#include "gtest/gtest.h"


TEST(XMLConfigurationShould, ReturnSameServiceName){
  XMLFileConfiguration *cfg = new XMLFileConfiguration("/Users/Zied/Projects/STAR-MQ-Utility/configuration.xml");
  cfg->loadConfiguration();
  std::string expected = "Service1";
  std::string actual = cfg->getServiceConfiguration(expected)->getServiceName();
  EXPECT_EQ(expected,actual);
};

TEST(XMLConfigurationShould, ReturnSameServiceNumber){
  XMLFileConfiguration *cfg = new XMLFileConfiguration("/Users/Zied/Projects/STAR-MQ-Utility/configuration.xml");
  cfg->loadConfiguration();
  int expected = 2;
  int actual = cfg->getServiceConfigurationVector().size();
  EXPECT_EQ(expected,actual);
}

TEST(XMLConfigurationShould, ReturnSameQueueNumberForAService){
  XMLFileConfiguration *cfg = new XMLFileConfiguration("/Users/Zied/Projects/STAR-MQ-Utility/configuration.xml");
  cfg->loadConfiguration();
  int expected = 2;
  int actual = cfg->getServiceConfiguration("Service1")->getQueueMap().size();
  EXPECT_EQ(expected,actual);
}

TEST(XMLConfigurationShould, CatchExceptionFileNotFound){
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

TEST(XMLConfigurationShould, CatchExceptionFileEmpty){
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

TEST(XMLConfigurationShould, CatchExceptionFileCorrupted){
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

