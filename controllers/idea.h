#pragma once

#include <drogon/HttpController.h>

using namespace drogon;

class idea : public drogon::HttpController<idea>
{
  public:
    METHOD_LIST_BEGIN
    // use METHOD_ADD to add your custom processing function here;
    // METHOD_ADD(idea::get, "/{2}/{1}", Get); // path is /idea/{arg2}/{arg1}
    // METHOD_ADD(idea::your_method_name, "/{1}/{2}/list", Get); // path is /idea/{arg1}/{arg2}/list
    // ADD_METHOD_TO(idea::your_method_name, "/absolute/path/{1}/{2}/list", Get); // path is /absolute/path/{arg1}/{arg2}/list
    METHOD_ADD(idea::get, "/", Get,"IDlogin"); // path is /idea/{arg2}/{arg1}
    METHOD_ADD(idea::newidea, "/newidea", Get, "IDlogin");
    METHOD_ADD(idea::ideaInfo, "/{1}/ideainfo", Get, "IDlogin");
    METHOD_ADD(idea::edit, "{1}/edit", Get, "IDlogin");
    METHOD_ADD(idea::AddIdea, "/AddIdea", Post, "IDlogin");
    METHOD_ADD(idea::EditIdea, "/{1}/EditIdea", Post, "IDlogin");
    METHOD_ADD(idea::DeleteIdea, "/{1}/DeleteIdea", Get, "IDlogin");
    METHOD_ADD(idea::ChageState, "/changestate", Get, "IDlogin");
    METHOD_ADD(idea::IdeaAll, "/All", Get, "IDlogin");

    METHOD_LIST_END
    // your declaration of processing function maybe like this:
    // void get(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, int p1, std::string p2);
    // void your_method_name(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, double p1, int p2) const;
    void get(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);
    void newidea(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) const;
    void ideaInfo(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback, int Ideaid) const;
    void edit(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback, int Ideaid) const;
    void AddIdea(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) const;
    void EditIdea(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback, int Ideaid) const;
    void DeleteIdea(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback, int Ideaid) const;
    void ChageState(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) const;
    void IdeaAll(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) const;
    std::string IdeaListSQL() const;
    std::string StateListSQL() const;
    std::string IdeaAddSQL(std::string UserID, int Ideaid,bool deadlineExist) const;
    std::string IdeaEditSQL(std::string UserID, int Ideaid, bool deadlineExist) const;
    std::string IdeaDeleteSQL(std::string UserID, int Ideaid) const;
    std::string IdeaAllSQL(std::string tag,std::string Keyword, std::string IsComp, std::string sort_by) const;
    std::map<int, std::string> MakeStateList(std::string UserID) const;
    std::vector<std::string> MakeTagList(std::string UserID) const;
    std::tm TMFromSQLdata(std::string) const;
    std::string TMtoSQLdata() const;
};
