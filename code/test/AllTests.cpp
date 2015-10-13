#include "CppUTest/CommandLineTestRunner.h"
#include "CppUTest/MemoryLeakDetector.h"

int main(int ac, char** av)
{
    return CommandLineTestRunner::RunAllTests(ac, av);
}
