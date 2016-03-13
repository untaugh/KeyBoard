TESTS = PasswordTests.cpp RandomTests.cpp
TARGET = test.bin
INCLUDE = -I ../
CPPUTEST_HOME = ../../../../Software/cpputest
CPPFLAGS += -I$(CPPUTEST_HOME)/include
LIBRARIES = -L$(CPPUTEST_HOME)/lib -lCppUTest -lCppUTestExt
CXXFLAGS += -include $(CPPUTEST_HOME)/include/CppUTest/MemoryLeakDetectorNewMacros.h
CFLAGS += -include $(CPPUTEST_HOME)/include/CppUTest/MemoryLeakDetectorMallocMacros.h
test: $(TESTS) AllTests.cpp
	g++ ../password.c ../random.c $(TESTS) -Wall -Wextra AllTests.cpp -o $(TARGET) $(INCLUDE) $(CPPFLAGS) $(CFLAGS) $(LIBRARIES) 

.phony: clean

clean:
	rm -rf $(TARGET) 
