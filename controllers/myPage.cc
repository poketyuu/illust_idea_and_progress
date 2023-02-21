#include "myPage.h"
#define ID "loginUser"

// Add definition of your processing function here
void myPage::mypage(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    std::string UserID = req->session()->get<std::string>(ID);
    auto viewdata = HttpViewData();
    std::vector<std::tuple<int, std::string, int>> StateList;
    try{
        auto DBClient = drogon::app().getDbClient("default");
        auto StateListResult = DBClient->execSqlAsyncFuture("select * from state_view where id = $1", UserID).get();
        for (auto row : StateListResult)
        {
            if (row["turn"].as<int>() == -1){
                StateList.insert(StateList.begin(), {row["sid"].as<int>(), row["context"].as<std::string>(), row["count"].as<int>()});
                break;
            }
            StateList.push_back({row["sid"].as<int>(), row["context"].as<std::string>(), row["count"].as<int>()});
        }
        viewdata.insert("StateList", StateList);
        viewdata.insert("load", true);
        viewdata.insert("ID", UserID);
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
    std::vector<std::tuple<int, std::string,int>> StateList;
    int MaxSid = 1;
    // StateList.push_back({1, "完成"});
    auto viewdata = HttpViewData();
    try{
        auto DBClient = drogon::app().getDbClient("default");
        auto StateListResult = DBClient->execSqlAsyncFuture("select * from state_view where id = $1",UserID).get();
        for(auto row:StateListResult){
            if(row["turn"].as<int>()==0) break;
            StateList.push_back({row["sid"].as<int>(), row["context"].as<std::string>(),row["count"].as<int>()});
        }
        auto MaxSidResult = DBClient->execSqlAsyncFuture("select sid from state where id = $1 order by sid desc", UserID).get();
        MaxSid = MaxSidResult[0]["sid"].as<int>();
    }
    catch (const std::exception &e)
    {
    }
    //StateList.push_back({0, "未着手"});
    viewdata.insert("StateList", StateList);
    viewdata.insert("MaxSid", MaxSid);
    callback(HttpResponse::newHttpViewResponse("StateEdit.csp", viewdata));
}
void myPage::ChangeStates(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback){
    std::string UserID = req->session()->get<std::string>(ID);
    auto paras = req->getParameters();
    std::vector<int> turns = turnVectorFromString(paras["sid-list"]);
    try{
        int Maxturn = turns.size();
        int MaxSid;
        std::vector<int> oldsidList;
        auto DBClient = drogon::app().getDbClient("default");
        auto OldSidResult = DBClient->execSqlAsyncFuture("select sid from state where id = $1 order by sid desc", UserID).get();
        for(auto row:OldSidResult){
            oldsidList.push_back(row["sid"].as<int>());
        }
        MaxSid = oldsidList[0];
        for (int i = 0; i < turns.size(); i++)
        {
            int sid = turns[i];
            auto newturn = Maxturn - i;
            auto itr = std::find(oldsidList.begin(), oldsidList.end(), sid);
            if(itr == oldsidList.end()){
                if(sid==-1){
                    MaxSid++;
                    sid = MaxSid;
                }
                DBClient->execSqlAsyncFuture("insert into state values($1,$2,$3,$4)", sid, paras["state-" + std::to_string(i)], newturn ,UserID);
            }else{
                DBClient->execSqlAsyncFuture("update state set turn = $1, context = $2 where sid = $3 and id = $4", newturn, paras["state-" + std::to_string(i)], sid, UserID);
                oldsidList.erase(itr);
            }
        }
        for(auto sid:oldsidList){
            if(sid<2) break;
            DBClient->execSqlAsyncFuture("delete from state where sid = $1 and id = $2", sid, UserID);
        }
    }
    catch (std::exception &e)
    {
    }
    auto viewdata = HttpViewData();
    viewdata.insert("newpage", "/mypage/");
    auto res = drogon::HttpResponse::newHttpViewResponse("PageTransition.csp", viewdata);
    callback(res);
}
std::vector<int> myPage::turnVectorFromString(std::string turnString) const{
    std::vector<int> turns;
    std::stringstream ss{turnString};
    std::string str;
    while (std::getline(ss,str,','))
    {
        if(str.empty()) break;
        turns.push_back(std::stoi(str));
    }
    return turns;
}