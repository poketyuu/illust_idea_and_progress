#pragma once

#include <drogon/HttpController.h>

using namespace drogon;

class IdeaChoice : public drogon::HttpController<IdeaChoice>
{
  public:
    METHOD_LIST_BEGIN
    // use METHOD_ADD to add your custom processing function here;
    // METHOD_ADD(IdeaChoice::get, "/{2}/{1}", Get); // path is /IdeaChoice/{arg2}/{arg1}
    // METHOD_ADD(IdeaChoice::your_method_name, "/{1}/{2}/list", Get); // path is /IdeaChoice/{arg1}/{arg2}/list
    // ADD_METHOD_TO(IdeaChoice::your_method_name, "/absolute/path/{1}/{2}/list", Get); // path is /absolute/path/{arg1}/{arg2}/list
    METHOD_ADD(IdeaChoice::get, "/{1}/{2}", Get, "IDlogin");
    METHOD_ADD(IdeaChoice::setPage, "/{1}/{2}", Post, "IDlogin");

    METHOD_LIST_END
    // your declaration of processing function maybe like this:
    // void get(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, int p1, std::string p2);
    // void your_method_name(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, double p1, int p2) const;
    void get(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback, int dojinID, int pageNum);
    void setPage(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback, int dojinID, int pageNum);
    std::string GetIdeaList() const;
    std::string SetPage() const;
};
