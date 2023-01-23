#pragma once

#include <drogon/HttpController.h>

using namespace drogon;

class dojinshi : public drogon::HttpController<dojinshi>
{
  public:
    METHOD_LIST_BEGIN
    // use METHOD_ADD to add your custom processing function here;
    // METHOD_ADD(dojinshi::get, "/{2}/{1}", Get); // path is /dojinshi/{arg2}/{arg1}
    // METHOD_ADD(dojinshi::your_method_name, "/{1}/{2}/list", Get); // path is /dojinshi/{arg1}/{arg2}/list
    // ADD_METHOD_TO(dojinshi::your_method_name, "/absolute/path/{1}/{2}/list", Get); // path is /absolute/path/{arg1}/{arg2}/list
    METHOD_ADD(dojinshi::get, "/", Get, "IDlogin");
    METHOD_ADD(dojinshi::getDojin, "/{1}", Get, "IDlogin");

    METHOD_LIST_END
    // your declaration of processing function maybe like this:
    // void get(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, int p1, std::string p2);
    // void your_method_name(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, double p1, int p2) const;

    void get(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback);
    void getDojin(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback, int did);
    std::string getDojinshiSQL() const;
    std::string getDojinDetailSQL() const;
    int DojinpageSum(std::string id, int did) const;
    int PerfectPageSum(std::string id, int did) const;
    std::map<int, std::string> MakeStateList(std::string UserID) const;
};
