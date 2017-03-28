
#include "test.h"
#include <iostream>

using namespace std;

int fact(int number)
{
	return number <= 1 ? 1 : fact(number - 1) * number;
}

IMPLEMENT_TEST_SYSTEM

DECLARE_TEST_CASE(math)


TEST_BEGIN(fact_1, math)
{
	test_assert(fact(31) == 1);
}
	
TEST_BEGIN(fact_2, math)
{
	test_assert(fact(12) == 34);
}

TEST_BEGIN(fact_3, math)
{
	test_assert(fact(2) == 2);
}

DECLARE_TEST_CASE(string)

TEST_BEGIN(size, string)
{
	std::string test{ "1234567890" };
	test_assert(test.size() == 10);
}

int main()
{
	TestCase::SetMessageHandler([](const char* message) { OutputDebugStringA(message); });
	TestCase::Run();

	return 0;
}