#include "CppUTest/TestHarness.h"
//#include "CppUTest/MemoryLeakDetectorMallocMacros.h"
#include "password.h"
#include <string.h>

TEST_GROUP(Password)
{
  void setup()
  {
    password_init(10);
  }

  void teardown()
  {
    password_clear();
  }
  
};

TEST(Password, size)
{
  LONGS_EQUAL(0,password_size());
  password_add('a');
  LONGS_EQUAL(1,password_size());
  password_add('b');
  LONGS_EQUAL(2,password_size());
}

TEST(Password, getPointer)
{
  STRCMP_EQUAL("",(const char*)password_getpointer());
    
  password_add('g');
  password_add('H');
  password_add('r');
  password_add('o');
  
  STRCMP_EQUAL("gHro",(const char*)password_getpointer());
}

/* Write past buffer size. */
TEST(Password, BufferOverflow)
{
  int i;

  password_add('a');
  
  for (i=0; i<200; i++)
    {
      password_add('w');
    }

  
    STRCMP_EQUAL("awwwwwwww",(const char*)password_getpointer());
}

TEST(Password, Clear)
{

  password_add('a');
  password_add('d');
  password_add('f');

  password_clear();
  
  password_init(8);

  STRCMP_EQUAL("",(const char*)password_getpointer());

}

TEST(Password, InitTwice)
{
  password_add('a');
  password_add('d');
  password_add('f');

  // the second init will clear the password  
  password_init(8);

  STRCMP_EQUAL("",(const char*)password_getpointer());
}

TEST(Password, WriteNoInit)
{
  //password_clear();  // is this the test? 
  //password_add('a');
}

TEST(Password, GetCharacter)
{
  char * pass = (char *)password_getpointer();

  strcpy(pass, (char *)"This!.....");

  LONGS_EQUAL(10,password_size());

  password_begin();

  LONGS_EQUAL('T',password_getchar());
  LONGS_EQUAL('h',password_getchar());
  LONGS_EQUAL('i',password_getchar());

  for (int i = 0; i < 6; i++)
    {
      password_getchar();
    }
  
  LONGS_EQUAL('.',password_getchar());

  LONGS_EQUAL(0,password_getchar());
  LONGS_EQUAL(0,password_getchar());
  LONGS_EQUAL(0,password_getchar());

}

//TODO: Write password to eeprom
//TODO: Read from eeprom
