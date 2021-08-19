#pragma once

#include <userver/storages/redis/subscribe_client.hpp>
#include <userver/utest/utest.hpp>

#include <gmock/gmock.h>

namespace storages::redis {

/// Mocked SubscribeClient. Mocking is done with google mock. Please
/// see GMock documentation on how to use this class. (Hint: GMock is
/// really powerful)
class MockSubscribeClient : public SubscribeClient {
 public:
  virtual ~MockSubscribeClient() = default;
  MOCK_METHOD(SubscriptionToken, Subscribe,
              (std::string channel,
               SubscriptionToken::OnMessageCb on_message_cb,
               const ::redis::CommandControl& command_control),
              (override));
  MOCK_METHOD(SubscriptionToken, Psubscribe,
              (std::string pattern,
               SubscriptionToken::OnPmessageCb on_pmessage_cb,
               const ::redis::CommandControl& command_control),
              (override));
};

/// Mocked SubscriptionTokenImplBase. Although one can used it by itself,
/// it mostly for use with MockSubscribeClient. Here is a small example on
/// how to use this class
/// @snippet storages/redis/test/subscribe_client_mock_test.cpp SbTknExmpl1
class MockSubscriptionTokenImpl : public SubscriptionTokenImplBase {
 public:
  virtual ~MockSubscriptionTokenImpl() = default;

  MOCK_METHOD(void, SetMaxQueueLength, (size_t length), (override));

  MOCK_METHOD(void, Unsubscribe, (), (override));
};

}  // namespace storages::redis