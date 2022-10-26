#include "idea.h"
#include "IdeaClass.h"
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
            std::string deadline = row['deadline'].as<std::string>();
            if (!deadline.empty()){
                time_t timenow = time(nullptr);
            }
            stepidea.push_back(std::unordered_map<std::string, std::string>{{"chara", row["chara"].as<std::string>()}, {"explain", row["explain"].as<std::string>()}, {"deadline", row["deadline"].as<std::string>()}});
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
std::string idea::IdeaListSQL() const{
    return std::string("select * from ideaview where id = $1");
}
std::string idea::StateListSQL() const{
    return std::string("select * from state where id = $1 order by turn desc");
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
