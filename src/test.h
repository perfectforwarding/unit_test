
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

#include "windows.h"

class TestCase
{
public:

	typedef void(*MessageHandler)(const char* message);

	TestCase(const std::string& name) : m_name(name)
	{
	}

	static MessageHandler	GetMessageHandler()				{ return s_messageHandler; }
	static void				SetMessageHandler(MessageHandler _mh) { s_messageHandler = _mh; }

	void	PrintDebugString(const char* msg) const
	{
		if(s_messageHandler)
			s_messageHandler(msg);
	}

	void	ReportFailure(const char* condition, const char* msg, const char* file, int line)
	{
		PrintDebugString("Failed!\n");
		std::stringstream ss;
		//ss << line << '\n';
		ss << file << "(" << line << "): " << condition << '\n';
		PrintDebugString(ss.str().c_str());
	};

	void	ReportSucceeded(const char* condition, const char* msg, const char* file, int line)
	{
		PrintDebugString("Succeeded!\n");
	}

	void	AddCase(const std::string& case_name, TestCase* test_case)
	{
		const auto it = find_if(s_testCases.begin(), s_testCases.end(), [&case_name](const std::pair<std::string, TestCase*>& elem) -> bool
		{
			return elem.first == case_name;
		});
		if (it == s_testCases.end())
		{
			s_testCases.emplace_back(std::string{ case_name }, test_case);
		}
	}

	static void	Run()
	{
		std::for_each(s_testCases.begin(), s_testCases.end(), [](const std::pair<std::string, TestCase*>& elem)
		{
			elem.second->Execute();
		});
	}

	virtual void Execute() {}

private:

	static MessageHandler s_messageHandler;
	std::string			m_name;

	static std::vector<std::pair<std::string, TestCase*>> s_testCases;
};

#define IMPLEMENT_TEST_SYSTEM \
	vector<pair<string, TestCase*>> TestCase::s_testCases; \
	TestCase::MessageHandler		TestCase::s_messageHandler;

#define DECLARE_TEST_CASE(NAME) \
	class TestCaseConcrete_##NAME : public TestCase \
	{ public: \
		TestCaseConcrete_##NAME() : TestCase(#NAME) {} \
		void AddTest(const std::string& name, TestCaseConcrete_##NAME* f)\
		{ \
			m_tests.emplace_back(name, f); \
		} \
		virtual void Execute() override \
		{\
			PrintDebugString("Starting test case ");\
			PrintDebugString(#NAME);\
			PrintDebugString("\n");\
			for_each(begin(m_tests), end(m_tests), [this](const std::pair<std::string, TestCaseConcrete_##NAME*>& element)\
			{\
				PrintDebugString("Runing test ");\
				PrintDebugString(element.first.c_str());\
				PrintDebugString("\n");\
				element.second->run();\
			});\
			PrintDebugString("Finished test case ");\
			PrintDebugString(#NAME);\
			PrintDebugString("\n");\
		}\
		virtual void run() {} \
		static std::vector<std::pair<std::string, TestCaseConcrete_##NAME*>> m_tests; \
	};\
	std::vector<std::pair<std::string, TestCaseConcrete_##NAME*>> TestCaseConcrete_##NAME::m_tests;

#define TEST_BEGIN(NAME, CASENAME) \
	class test_##NAME : public TestCaseConcrete_##CASENAME { public: \
		test_##NAME() { \
			TestCase::AddCase(#CASENAME, this); \
			TestCaseConcrete_##CASENAME::AddTest(#NAME, this); \
		} \
		virtual void run() override; \
	};\
	static test_##NAME test_##NAME##_instance; \
	void test_##NAME::run() \

#define test_assert(cond) \
	do { \
		if(!(cond)) { \
			ReportFailure(#cond, #cond, __FILE__, __LINE__); \
		} else { \
			ReportSucceeded(#cond, #cond, __FILE__, __LINE__); \
		}\
	} while(0)