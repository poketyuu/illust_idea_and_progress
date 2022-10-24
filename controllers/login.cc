#include "login.h"

// Add definition of your processing function here
void login::logincheck(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) const
{
    std::string newpage = "/login/";
    std::string UserID = req->getParameter("id");
    try
    {
        req->session()->insert("loginState", false);
        auto result = drogon::app().getDbClient("default")->execSqlAsyncFuture(LoginSQL(), UserID).get();
        if (result.size() > 0)
        {
            req->session()->insert("loginState", true);
            req->session()->insert("loginUser", UserID);
            newpage = "/idea/";
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    auto viewdata = HttpViewData();
    viewdata.insert("newpage", newpage);
    callback(HttpResponse::newHttpViewResponse("PageTransition.csp",viewdata));
}
void login::loginform(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) const
{
    callback(HttpResponse::newHttpViewResponse("LoginForm.csp"));
}
std::string login::LoginSQL() const
{
    return std::string("SELECT id FROM illustrator where id = $1");
}
