#pragma once

#include <drogon/HttpController.h>

using namespace drogon;

class myPage : public drogon::HttpController<myPage>
{
  public:
    METHOD_LIST_BEGIN
    // use METHOD_ADD to add your custom processing function here;
    // METHOD_ADD(myPage::get, "/{2}/{1}", Get); // path is /myPage/{arg2}/{arg1}
    // METHOD_ADD(myPage::your_method_name, "/{1}/{2}/list", Get); // path is /myPage/{arg1}/{arg2}/list
    // ADD_METHOD_TO(myPage::your_method_name, "/absolute/path/{1}/{2}/list", Get); // path is /absolute/path/{arg1}/{arg2}/list
    METHOD_ADD(myPage::mypage, "/", Get, "IDlogin");
    METHOD_ADD(myPage::EditState, "/EditState", Get, "IDlogin");
    METHOD_ADD(myPage::ChangeStates, "/ChangeStates", Post, "IDlogin");

    METHOD_LIST_END
    // your declaration of processing function maybe like this:
    // void get(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, int p1, std::string p2);
    // void your_method_name(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, double p1, int p2) const;
    void mypage(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);
    void EditState(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);
    void ChangeStates(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);
    std::vector<int> turnVectorFromString(std::string turnString) const;
};
