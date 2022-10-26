#pragma
#include <iostream>
#include <chrono>
using namespace std;
class IdeaClass
{
private:
    /* data */
public:
    string Userid;
    string chara;
    string expain;
    time_t draft;
    string context;
    int turn;
    IdeaClass(/* args */);
    ~IdeaClass();
};

IdeaClass::IdeaClass(/* args */)
{
}

IdeaClass::~IdeaClass()
{
}
