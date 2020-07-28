#include "gtest/gtest.h"
#include "src/lib/MQUtility.h"

TEST(MQScannerShould, callInitialiseMQ){
  testing::StrictMock<MQClient> client;
  EXPECT_CALL(client, initialise(testing::_)).Times(1).WillOnce(testing::Return(true));
  MQUtility::MQUtility* util = new MQUtility::MQUtility("/Users/Zied/Projects/STAR-MQ-Utility/configuration.xml","Service1",MQUtility::XML_MODE);
  util->run();
};

// TEST(MQScannerShould, connectToMQQueueManager){
//   MQClient client;
//   ON_CALL(client, initialise(testing::_)).WillByDefault(testing::Return(true));
//   MQConnection* mqcExpected = new MQConnection();
//   mqcExpected->setReturnCode(true);
//   EXPECT_CALL(client, connect(testing::_)).Times(1).WillOnce(testing::Return(mqcExpected));
//   MQScanner* scanner = new MQScanner(&client);
//   int rc = scanner->checkConnectivity();
//   EXPECT_EQ(rc, 1);
// };
