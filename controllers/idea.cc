#include "idea.h"
#include "IdeaClass.h"
#include <regex>
#include <time.h>
#include <stdio.h>
#define ID "loginUser"

// Add definition of your processing function here
void idea::get(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    try
    {
        auto UserID = req->session()->get<std::string>(ID);
        auto result = drogon::app().getDbClient("default")->execSqlAsyncFuture(IdeaListSQL(), UserID).get();
        std::vector<std::pair<int,
                              std::vector<std::unordered_map<std::string, std::string>>>>
            IdeaList;
        //各状態ごとに状態番号とideaのvectorを作成
        int turnCheck = 0;
        bool firstcheck = true;
        std::vector<std::unordered_map<std::string, std::string>> stepidea;
        for (auto row : result)
        {
            std::string cardtype = R"("card border-secondory mb-3 h-100")";
            std::string cardLink = std::string("\""+row["iid"].as<std::string>()+"/ideainfo\"");
            int turn = row["turn"].as<int>();
            if (turnCheck != turn)
            {
                if(!firstcheck||turn<0)
                    IdeaList.push_back(std::pair<int,
                                                 std::vector<std::unordered_map<std::string, std::string>>>{turnCheck, stepidea});
                stepidea.clear();
                turnCheck = turn;
                firstcheck = false;
            }
            std::string deadline = row["deadline"].as<std::string>();
            if (!deadline.empty()){
                std::time_t timenow = time(nullptr);
                std::tm dltm = TMFromSQLdata(deadline);
                std::time_t dlt = std::mktime(&dltm);
                if(dlt < timenow){
                    cardtype = R"("card text-white bg-dark bg-opacity-75 mb-3 h-100")";
                }else{
                    if(dlt-timenow < 60*60*24*7){
                        cardtype = R"("card text-white bg-danger mb-3 h-100")";
                    }else if (dlt-timenow < 60*60*24*30)
                    {
                        cardtype = R"("card text-dark bg-warning bg-opacity-50 mb-3 h-100")";
                    }else{
                        cardtype = R"("card text-dark bg-info bg-opacity-25 mb-3 h-100")";
                    }
                    
                }
            }
            stepidea.push_back(std::unordered_map<std::string, std::string>
            {{"title", row["title"].as<std::string>()},
             {"explain", row["explain"].as<std::string>()}, 
             {"deadline", deadline},
             {"cardtype",cardtype},
             {"cardLink",cardLink},
             {"ideaid",row["iid"].as<std::string>()}
             });
        }
        IdeaList.push_back(std::pair<int,
                                     std::vector<std::unordered_map<std::string, std::string>>>{turnCheck, stepidea});
        auto viewData = HttpViewData();
        viewData.insert("idea", IdeaList);
        viewData.insert("state", MakeStateList(UserID));
        callback(HttpResponse::newHttpViewResponse("IdeaCsp.csp", viewData));
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
}
void idea::newidea(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) const
{
    callback(HttpResponse::newHttpViewResponse("NewIdea.csp"));
}
void idea::AddIdea(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) const
{
    try{
        auto para = req->getParameters();
        auto userID = req->session()->get<std::string>(ID);
        auto DBclient = drogon::app().getDbClient("default");
        auto GetIidMaxSql = DBclient->execSqlAsyncFuture("SELECT iid from idea where id = $1 order by iid desc", userID).get();
        int NewIid = 0;
        if (GetIidMaxSql.size() != 0)
        {
            NewIid = GetIidMaxSql[0]["iid"].as<int>();
        }
        NewIid++;
        std::string deadline = para["idea_deadline"];
        if (deadline.empty())
        {
            auto result = DBclient->execSqlAsyncFuture(IdeaAddSQL(userID, NewIid, false),
                                                       para["idea_title"], para["idea_explain"]).get();
        }
        else
        {
            auto result = DBclient->execSqlAsyncFuture(IdeaAddSQL(userID,NewIid,true),
            para["idea_title"],para["idea_explain"],deadline).get();
        }
        auto progress = DBclient->execSqlAsyncFuture("INSERT into progress values($1,$2,0)", userID, NewIid).get();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
    auto viewdata = HttpViewData();
    viewdata.insert("newpage", "/idea/");
    auto res = drogon::HttpResponse::newHttpViewResponse("PageTransition.csp", viewdata);
    callback(res);
}
void idea::ideaInfo(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback,int Ideaid) const
{
    try
    {
        auto userID = req->session()->get<std::string>(ID);
        auto DBclient = drogon::app().getDbClient("default");
        auto result = DBclient->execSqlAsyncFuture("SELECT * FROM idea WHERE id = $1 AND iid = $2", userID, Ideaid).get();
        auto viewdata = HttpViewData();
        viewdata.insert("title", result[0]["title"].as<std::string>());
        viewdata.insert("explain", result[0]["explain"].as<std::string>());
        viewdata.insert("deadline", result[0]["deadline"].as<std::string>());
        viewdata.insert("today", TMtoSQLdata());
        callback(drogon::HttpResponse::newHttpViewResponse("IdeaMenu.csp", viewdata));
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        auto viewdata = HttpViewData();
        viewdata.insert("newpage", "/idea/");
        auto res = drogon::HttpResponse::newHttpViewResponse("PageTransition.csp", viewdata);
        callback(res);
    }
    
}
void idea::EditIdea(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback, int Ideaid) const
{
    auto UserID = req->session()->get<std::string>(ID);
    auto para = req->getParameters();
    try
    {
        auto DBclient = drogon::app().getDbClient("default");
        std::string deadline = para["idea_deadline"];
        if (deadline.empty())
        {
            auto result = DBclient->execSqlAsyncFuture(IdeaEditSQL(UserID, Ideaid, false),
                                                       para["idea_title"], para["idea_explain"])
                              .get();
        }
        else
        {
            auto result = DBclient->execSqlAsyncFuture(IdeaEditSQL(UserID, Ideaid, true),
                                                       para["idea_title"], para["idea_explain"], deadline)
                              .get();
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    auto viewdata = HttpViewData();
    viewdata.insert("newpage", "/idea/");
    auto res = drogon::HttpResponse::newHttpViewResponse("PageTransition.csp", viewdata);
    callback(res);
}
void idea::DeleteIdea(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback, int Ideaid) const
{
    auto UserID = req->session()->get<std::string>(ID);
    auto viewdata = HttpViewData();
    try
    {
        auto DBclient = drogon::app().getDbClient("default");
        auto result = DBclient->execSqlAsyncFuture("DELETE FROM idea WHERE id = $1 AND iid = $2", UserID, Ideaid).get();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    viewdata.insert("newpage", "/idea/");
    auto res = drogon::HttpResponse::newHttpViewResponse("PageTransition.csp", viewdata);
    callback(res);
}
void idea::ChageState(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) const
{
    auto UserID = req->session()->get<std::string>(ID);
    auto para = req->getParameters();
    try
    {
        auto DBclient = drogon::app().getDbClient("default");
        auto stateid = DBclient->execSqlAsyncFuture("SELECT * FROM state where id = $1 AND turn = $2", UserID, std::stoi(para["progress_turn"])).get()[0]["sid"].as<int>();
        auto result = DBclient->execSqlAsyncFuture("UPDATE progress SET sid = $1 WHERE id = $2 AND iid = $3", stateid, UserID, std::stoi(para["progress_iid"])).get();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    auto viewdata = HttpViewData();
    viewdata.insert("newpage", "/idea/");
    auto res = drogon::HttpResponse::newHttpViewResponse("PageTransition.csp", viewdata);
    callback(res);
}
std::string idea::IdeaListSQL() const
{
    return std::string("select * from ideaview where id = $1 order by turn desc, deadline,iid");
}
std::string idea::StateListSQL() const{
    return std::string("select * from state where id = $1 order by turn desc");
}
std::string idea::IdeaAddSQL(std::string UserID,int IdeaID,bool deadlineExist) const
{
    std::string newSQL;
    if(deadlineExist){
        newSQL=std::string("insert into idea values('" + UserID + "'," + std::to_string(IdeaID) + ",$1,$2,'" +TMtoSQLdata() + "',$3)");
    }
    else
    {
        newSQL = std::string("insert into idea values('" + UserID + "'," + std::to_string(IdeaID) + ",$1,$2,'" + TMtoSQLdata() + "',NULL)");
    }
    return newSQL;
}
std::string idea::IdeaEditSQL(std::string UserID, int IdeaID, bool deadlineExist) const
{
    std::string newSQL;
    if (deadlineExist)
    {
        newSQL = std::string("update idea set title = $1,explain = $2,deadline = $3 where id = '" + UserID + "' and iid = " + std::to_string(IdeaID));
    }
    else
    {
        newSQL = std::string("update idea set title = $1,explain = $2,deadline = NULL where id = '" + UserID + "' and iid = " + std::to_string(IdeaID));
    }
    return newSQL;
}
std::map<int, std::string> idea::MakeStateList(std::string UserID) const
{
    std::map<int,std::string> statelist;
    try
    {
        auto result = drogon::app().getDbClient("default")->execSqlAsyncFuture(StateListSQL(), UserID).get();
        for(auto row:result){
            statelist.insert(std::pair<int,std::string>(row["turn"].as<int>(), row["context"].as<std::string>()));
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    return statelist;
}
std::tm idea::TMFromSQLdata(std::string datastring) const
{
    std::smatch results;
    std::string pattern = R"((\d{4})-(\d{2})-(\d{2}))";
    if (std::regex_match(datastring, results, std::regex(pattern)))
    {
        int year = std::stoi(results[1].str());
        int month = std::stoi(results[2].str());
        int day = std::stoi(results[3].str());
        return std::tm{0, 0, 0, day, month - 1, year - 1900};
    }
    return std::tm{0, 0, 0, 0, 0, 0};
}
std::string idea::TMtoSQLdata() const
{
    std::time_t timenow = time(NULL);
    struct tm local;
    auto lt = localtime_r(&timenow, &local);
    char buf[128];
    strftime(buf, sizeof(buf), R"(%Y-%m-%d)", &local);
    return std::string(buf);
}
