#include "dojinshi.h"
#define ID "loginUser"

// Add definition of your processing function here
void dojinshi::get(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    std::string UserID = req->session()->get<std::string>(ID);
    auto viewdata = HttpViewData();
    std::vector<std::unordered_map<std::string, std::string>> DojinList;
    try
    {
        auto DBClient = drogon::app().getDbClient("default");
        auto result = DBClient->execSqlAsyncFuture(getDojinshiSQL(), UserID).get();
        for(auto row:result){
            int did = row["did"].as<int>();
            int page = DojinpageSum(UserID, did);
            int perfect = PerfectPageSum(UserID, did);
            std::unordered_map<std::string, std::string> dojin;
            dojin.emplace("link", std::to_string(did));
            dojin.emplace("name", row["name"].as<std::string>());
            dojin.emplace("deadline", row["soul_deadline"].as<std::string>());
            dojin.emplace("pagenum", row["pagenum"].as<std::string>());
            dojin.emplace("page",std::to_string(page));
            dojin.emplace("perfect", std::to_string(perfect));
            DojinList.push_back(dojin);
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    viewdata.insert("dojindata", DojinList);
    callback(HttpResponse::newHttpViewResponse("DojinList.csp", viewdata));
}
void dojinshi::getDojin(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback,int did)
{
    std::string UserID = req->session()->get<std::string>(ID);
    auto viewdata = HttpViewData();
    try
    {
        auto DBClient = drogon::app().getDbClient("default");
        auto dojindata = DBClient->execSqlAsyncFuture("SELECT * from dojinshi where id = $1 AND did = $2", UserID, did).get();
        auto pagedata = DBClient->execSqlAsyncFuture("SELECT * from dojinview where id = $1 and did = $2", UserID, did).get();
        if(dojindata.size() >0){
            int pagenum = dojindata[0]["pagenum"].as<int>();
            viewdata.insert("title", dojindata[0]["name"].as<std::string>());
            viewdata.insert("deadline", dojindata[0]["soul_deadline"].as<std::string>());
            viewdata.insert("pagenum", pagenum);
            std::vector<std::unordered_map<std::string, std::any>> pages;
            for (int i = 0; i < pagenum; i++)
            {
                pages.push_back(std::unordered_map<std::string, std::any>
                {{"double",false}}
                );
            }
            for(auto page:pagedata){
                int num = page["num"].as<int>();
                std::string id = page["iid"].as<std::string>();
                if(id.empty()){
                    pages[num % pagenum].emplace("title", std::string("no illust"));
                    pages[num%pagenum].emplace("comp", page["comp"].as<bool>());
                    pages[num % pagenum].insert_or_assign("double", page["double"].as<bool>());
                }else{
                    pages[num%pagenum].emplace("title", page["title"].as<std::string>());
                    pages[num%pagenum].emplace("comp", page["comp"].as<bool>());
                    pages[num%pagenum].emplace("turn", page["turn"].as<int>());
                    pages[num % pagenum].insert_or_assign("double", page["double"].as<bool>());
                    pages[num % pagenum].emplace("deadline", page["deadline"].as<std::string>());
                    pages[num%pagenum].emplace("iid",id);
                }
            }
            viewdata.insert("pagedata", pages);
            viewdata.insert("state", MakeStateList(UserID));
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    callback(HttpResponse::newHttpViewResponse("DojinMenu.csp", viewdata));
}
std::string dojinshi::getDojinshiSQL() const
{
    return std::string("SELECT * FROM dojinshi WHERE id = $1 ORDER BY soul_deadline");
}
std::string dojinshi::getDojinDetailSQL() const
{
    return std::string("SELECT * FROM page WHERE id = $1 AND did = $2 ORDER BY num");
}
int dojinshi::DojinpageSum(std::string id, int did) const
{
    int page = 0;
    try
    {
        auto DBClient = drogon::app().getDbClient("default");
        auto illustnum = DBClient->execSqlAsyncFuture("SELECT count(*) FROM dojinview WHERE id = $1 AND did = $2", id, did).get()[0]["count"].as<int>();
        auto doublenum = DBClient->execSqlAsyncFuture("SELECT count(*) FROM dojinview WHERE id = $1 AND did = $2 AND double = true", id, did).get()[0]["count"].as<int>();
        page = illustnum + doublenum;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    return page;
}
int dojinshi::PerfectPageSum(std::string id, int did) const
{
    int page = 0;
    try
    {
        auto DBClient = drogon::app().getDbClient("default");
        auto illustnum = DBClient->execSqlAsyncFuture("SELECT count(*) FROM dojinview WHERE id = $1 AND did = $2 AND comp='true'", id, did).get()[0]["count"].as<int>();
        auto doublenum = DBClient->execSqlAsyncFuture("SELECT count(*) FROM dojinview WHERE id = $1 AND did = $2 AND double = true AND comp='true'", id, did).get()[0]["count"].as<int>();
        page = illustnum + doublenum;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
    return page;
}
std::map<int, std::string> dojinshi::MakeStateList(std::string UserID) const
{
    std::map<int, std::string> statelist;
    try
    {
        auto result = drogon::app().getDbClient("default")->execSqlAsyncFuture("select * from state where id = $1 order by turn desc", UserID).get();
        for (auto row : result)
        {
            statelist.insert(std::pair<int, std::string>(row["turn"].as<int>(), row["context"].as<std::string>()));
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
    return statelist;
}
