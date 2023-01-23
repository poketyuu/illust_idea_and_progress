#include "myPage.h"
#define ID "loginUser"

// Add definition of your processing function here
void myPage::mypage(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    std::string UserID = req->session()->get<std::string>(ID);
    auto viewdata = HttpViewData();
    try{
        auto DBClient = drogon::app().getDbClient("default");
        auto totaldata = DBClient->execSqlAsyncFuture("select count(*) from ideaview where id = $1",UserID).get();
        int total = totaldata[0]["count"].as<int>();
        auto inworkdata = DBClient->execSqlAsyncFuture("select count(*) from ideaview where id = $1 and turn > 0", UserID).get();
        int inwork = inworkdata[0]["count"].as<int>();
        auto perfect = DBClient->execSqlAsyncFuture("select count(*) from ideaview where id = $1 and turn = -1", UserID).get();
        int comp = perfect[0]["count"].as<int>();
        viewdata.insert("load", true);
        viewdata.insert("ID", UserID);
        viewdata.insert("total", total);
        viewdata.insert("inwork", inwork);
        viewdata.insert("comp", comp);
    }
    catch (const std::exception &e)
    {
        viewdata.insert("load", false);
    }
    callback(HttpResponse::newHttpViewResponse("myPage.csp", viewdata));
}
void myPage::EditState(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    std::string UserID = req->session()->get<std::string>(ID);
    auto viewdata = HttpViewData();
}