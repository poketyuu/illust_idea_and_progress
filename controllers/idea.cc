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
            std::string cardtype = R"("card card-nolimit")";
            std::string cardLink = std::string("\""+row["iid"].as<std::string>()+"/ideainfo\"");
            int turn = row["turn"].as<int>();
            if (turnCheck != turn)
            {
                if(turn < 0) break;
                if(!firstcheck)
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
                    cardtype = R"("card card-out")";
                }else{
                    if(dlt-timenow < 60*60*24*7){
                        cardtype = R"("card card-1week")";
                    }else if (dlt-timenow < 60*60*24*30)
                    {
                        cardtype = R"("card card-1month")";
                    }else{
                        cardtype = R"("card card-limit")";
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
        auto result = DBclient->execSqlAsyncFuture("SELECT * FROM ideaview WHERE id = $1 AND iid = $2", userID, Ideaid).get();
        auto viewdata = HttpViewData();
        viewdata.insert("title", result[0]["title"].as<std::string>());
        viewdata.insert("explain", result[0]["explain"].as<std::string>());
        viewdata.insert("deadline", result[0]["deadline"].as<std::string>());
        viewdata.insert("turn", result[0]["turn"].as<int>());
        viewdata.insert("today", TMtoSQLdata());
        viewdata.insert("StateList", MakeStateList(userID));
        std::vector<std::string> taglist;
        auto taglistResult = DBclient->execSqlAsyncFuture("SELECT * FROM class WHERE id = $1 AND iid = $2", userID, Ideaid).get();
        for(auto tag:taglistResult){
            taglist.push_back(tag["name"].as<std::string>());
        }
        viewdata.insert("taglist", taglist);
        std::vector<std::string> AllTags;
        auto AllTagsResult = DBclient->execSqlAsyncFuture("SELECT * FROM tags WHERE id = $1 AND name NOT IN (SELECT name FROM class WHERE id = $1 AND iid = $2 order by name)", userID, Ideaid).get();
        for (auto tag : AllTagsResult)
        {
            AllTags.push_back(tag["name"].as<std::string>());
        }
        viewdata.insert("AllTags", AllTags);
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
void idea::edit(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback, int Ideaid) const
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
        callback(drogon::HttpResponse::newHttpViewResponse("ideaEdit.csp", viewdata));
    }
    catch (const std::exception &e)
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
void idea::IdeaAll(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) const
{
    try
    {
        auto UserID = req->session()->get<std::string>(ID);
        auto tag = req->getParameter("tag");
        auto sort = req->getParameter("sort_by");
        auto IsComp = req->getParameter("IsComp");
        auto search = req->getParameter("search");
        auto result = drogon::app().getDbClient("default")->execSqlAsyncFuture(IdeaAllSQL(tag,search, IsComp, sort), UserID).get();
        std::vector<std::unordered_map<std::string, std::string>> IdeaList;
        int sum = 0;
        for (auto row : result)
        {
            std::unordered_map<std::string, std::string> IdeaCard;
            std::string cardtype = R"("card card-nolimit")";
            std::string cardLink = std::string("\"" + row["iid"].as<std::string>() + "/ideainfo\"");
            std::string deadline = row["deadline"].as<std::string>();
            //カードの種類判定(締切からの期間を見る)
            if (!deadline.empty())
            {
                std::time_t timenow = time(nullptr);
                std::tm dltm = TMFromSQLdata(deadline);
                std::time_t dlt = std::mktime(&dltm);
                if (dlt < timenow)
                {
                    cardtype = R"("card card-out")";
                }
                else
                {
                    if (dlt - timenow < 60 * 60 * 24 * 7)
                    {
                        cardtype = R"("card card-1week")";
                    }
                    else if (dlt - timenow < 60 * 60 * 24 * 30)
                    {
                        cardtype = R"("card card-1month")";
                    }
                    else
                    {
                        cardtype = R"("card card-limit")";
                    }
                }
            }
            if(row["turn"].as<int>()<0) cardtype = R"("card card-perfect")";
            sum++;
            IdeaList.push_back(std::unordered_map<std::string, std::string>{
                {"title", row["title"].as<std::string>()},
                {"explain", row["explain"].as<std::string>()},
                {"state", row["context"].as<std::string>()},
                {"deadline", deadline},
                {"cardtype", cardtype},
                {"cardLink", cardLink}});
        }
        auto viewData = HttpViewData();
        viewData.insert("IdeaList", IdeaList);
        viewData.insert("TagList", MakeTagList(UserID));
        viewData.insert("sum", sum);
        viewData.insert("search", search);
        viewData.insert("IsComp", IsComp);
        viewData.insert("sort_by", sort);
        viewData.insert("Tag", tag);
        callback(HttpResponse::newHttpViewResponse("IdeaAll.csp", viewData));
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
}
void idea::AddTag(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback, int Ideaid) const{
    auto UserID = req->session()->get<std::string>(ID);
    auto newtag = req->getParameter("newtag");
    try{
        auto DBClient = drogon::app().getDbClient("default");
        auto TagExistCheck = DBClient->execSqlAsyncFuture("select exists (select * from tags where id = $1 and name = $2)",UserID,newtag).get();
        bool exists = TagExistCheck[0]["exists"].as<bool>();
        if(!exists)
            DBClient->execSqlAsyncFuture("insert into tags values($1,$2)", UserID, newtag);
        DBClient->execSqlAsyncFuture("insert into class values($1,$2,$3)", UserID, Ideaid, newtag);
    }
    catch (const std::exception &e)
    {
    }
    auto viewData = HttpViewData();
    viewData.insert("newpage", "/idea/" + std::to_string(Ideaid) + "/ideainfo");
    callback(HttpResponse::newHttpViewResponse("PageTransition.csp", viewData));
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
std::string idea::IdeaAllSQL(std::string tag,std::string Keyword, std::string IsComp, std::string sort_by) const {
    std::string select = "select * from ideaview";
    std::string where = " where id = $1";
    std::string order = " order by deadline,iid desc";
    if (!tag.empty())
    {
        if(tag=="未分類"){
            where = " where (id,iid) not in (select id,iid from class where id = $1) and id = $1";
        }
        else
        {
            where = " where (id,iid) in (select id,iid from class where id = $1 and name = '" + tag + "')";
        }
    }
    if (!Keyword.empty())
    {
        if(Keyword.find("'") == std::string::npos){
            where = where + " and title like '%" + Keyword + "%'";
        }
    }
    if(IsComp=="True"){
        where = where + " and turn = -1";
    }else if(IsComp=="False"){
        where = where + " and turn != -1";
    }
    if(sort_by =="latest"){
        order = " order by iid desc";
    }else if(sort_by =="previous"){
        order = " order by iid";
    }
    return select + where + order;
}
std::map<int, std::string> idea::MakeStateList(std::string UserID) const
{
    //Get State List order by turn
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
std::vector<std::string> idea::MakeTagList(std::string UserID) const{
    //Get User's tag List order by tagged idea amount not 0
    std::vector<std::string> tagList;
    tagList.push_back("未分類");
    try
    {
        auto result = drogon::app().getDbClient("default")->execSqlAsyncFuture("select id,name,count(*) from class where id = $1 group by id,name order by count desc",UserID).get();
        for(auto row:result){
            if(row["count"].as<int>()==0) break;
            tagList.push_back(row["name"].as<std::string>());
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
    return tagList;
}
std::tm idea::TMFromSQLdata(std::string datastring) const
{
    // Get time data from string(YYYY-MM-dd)
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
    //Get string(YYYY-MM-dd) from time
    std::time_t timenow = time(NULL);
    struct tm local;
    auto lt = localtime_r(&timenow, &local);
    char buf[128];
    strftime(buf, sizeof(buf), R"(%Y-%m-%d)", &local);
    return std::string(buf);
}
