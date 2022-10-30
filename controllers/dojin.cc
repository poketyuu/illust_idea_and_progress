#include "dojin.h"
#define ID "loginUser"

// Add definition of your processing function here
void dojin::get(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    auto UserID = req->session()->get<std::string>(ID);
    auto viewdata = HttpViewData();
    std::vector<std::unordered_map<std::string, any>> DojinData;
    try
    {
        auto DBclient = drogon::app().getDbClient("default");
        auto result = DBclient->execSqlAsyncFuture("SELECT * FROM dojinshi where id = $1", UserID).get();
        for(auto row:result){
            DojinData.push_back(std::unordered_map<std::string, any>{
                {"name",row["name"].as<std::string>()},
                {"deadline",row["soul_deadline"].as<std::string>()},
                {"page",row["pagenum"].as<int>()}
                });
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
}
