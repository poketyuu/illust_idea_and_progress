#pragma once

#include <drogon/HttpController.h>

using namespace drogon;

class login : public drogon::HttpController<login>
{
  public:
    METHOD_LIST_BEGIN
    // use METHOD_ADD to add your custom processing function here;
    // METHOD_ADD(login::get, "/{2}/{1}", Get); // path is /login/{arg2}/{arg1}
    // METHOD_ADD(login::your_method_name, "/{1}/{2}/list", Get); // path is /login/{arg1}/{arg2}/list
    // ADD_METHOD_TO(login::your_method_name, "/absolute/path/{1}/{2}/list", Get); // path is /absolute/path/{arg1}/{arg2}/list
    METHOD_ADD(login::loginform, "/", Get);
    METHOD_ADD(login::logincheck, "/login", Post);

    METHOD_LIST_END
    // your declaration of processing function maybe like this:
    // void get(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, int p1, std::string p2);
    // void your_method_name(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, double p1, int p2) const;
    void loginform(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) const;
    void logincheck(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) const;
    std::string LoginSQL() const;
};
