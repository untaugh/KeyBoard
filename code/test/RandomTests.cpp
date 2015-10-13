#include "CppUTest/TestHarness.h"
#include "random.h"

TEST_GROUP(Random)
{
  void setup()
  {
    // init random with pointer to a running timer
    random_init(&faketimer);
  }

  uint8_t faketimer;
};

TEST(Random, Getting)
{
  this->faketimer = 33;
  LONGS_EQUAL(33, random_get(255));
  this->faketimer = 44;
  LONGS_EQUAL(44, random_get(255));
}

TEST(Random, Limit)
{
  this->faketimer = 32;
  LONGS_EQUAL(32, random_get(65));
  this->faketimer = 66;
  LONGS_EQUAL(16, random_get(50));

}
