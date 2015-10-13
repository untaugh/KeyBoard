TESTS = PasswordTests.cpp RandomTests.cpp
INCLUDE = -I ../
CPPUTEST_HOME = /home/oscu/lib/cpputest
CPPFLAGS += -I$(CPPUTEST_HOME)/include
LIBRARIES = -L$(CPPUTEST_HOME)/lib -lCppUTest -lCppUTestExt
CXXFLAGS += -include $(CPPUTEST_HOME)/include/CppUTest/MemoryLeakDetectorNewMacros.h
CFLAGS += -include $(CPPUTEST_HOME)/include/CppUTest/MemoryLeakDetectorMallocMacros.h
test: $(TESTS) AllTests.cpp
	g++ ../password.c ../random.c $(TESTS) -Wall -Wextra AllTests.cpp -o test $(INCLUDE) $(CPPFLAGS) $(CFLAGS) $(LIBRARIES) 

.phony: clean

clean:
	rm -rf test 
