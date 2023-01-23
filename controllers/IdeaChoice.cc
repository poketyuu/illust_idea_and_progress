#include "IdeaChoice.h"
#define ID "loginUser"

// Add definition of your processing function here
void IdeaChoice::get(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback, int dojinID, int pageNum)
{
    std::string UserID = req->session()->get<std::string>(ID);
    auto viewdata = HttpViewData();
    try
    {
        auto DBClient = drogon::app().getDbClient("default");
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
}
void IdeaChoice::setPage(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback, int dojinID, int pageNum)
{
    std::string UserID = req->session()->get<std::string>(ID);
    auto viewdata = HttpViewData();
    try
    {
        auto DBClient = drogon::app().getDbClient("default");
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
}
std::string IdeaChoice::GetIdeaList() const
{
    return "";
}
std::string IdeaChoice::SetPage() const
{
    return "";
}
