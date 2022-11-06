#include "login.h"
#include "loginhelper.h"
#define LEN 16

// Add definition of your processing function here
void login::logincheck(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) const
{
    std::string newpage = "/login/login";
    std::string UserID = req->getParameter("id");
    std::string pass = req->getParameter("password");
    try
    {
        auto DBClient = drogon::app().getDbClient("default");
        auto result = DBClient->execSqlAsyncFuture(LoginSQL(), UserID).get();
        if (result.size() > 0)
        {
            std::string salt = result[0]["salt"].as<std::string>();
            std::string passsalt = salt + pass;
            std::array<u_char, SHA512_DIGEST_LENGTH> inputhash = {0};
            std::string inputDigest = createDigestArray<char>(passsalt.c_str(), static_cast<size_t>(passsalt.length()), inputhash);
            if(inputDigest == result[0]["digest"].as<std::string>()){
                //ログイン成功
                req->session()->insert("loginUser", UserID);
                auto viewdata = HttpViewData();
                viewdata.insert("newpage", "/idea/");
                callback(HttpResponse::newHttpViewResponse("PageTransition.csp", viewdata));
                return;
            }
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    auto viewdata = HttpViewData();
    viewdata.insert("message", "ログイン情報に誤りがあります");
    callback(HttpResponse::newHttpViewResponse("LoginForm.csp",viewdata));
}
void login::loginform(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) const
{
    auto viewdate = HttpViewData();
    viewdate.insert("message", "");
    callback(HttpResponse::newHttpViewResponse("LoginForm.csp",viewdate));
}
void login::NewUser(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) const
{
    auto viewdate = HttpViewData();
    viewdate.insert("message", "");
    callback(HttpResponse::newHttpViewResponse("NewUser.csp",viewdate));
}
void login::addUser(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) const
{
    std::string UserID = req->getParameter("new_id");
    std::string password = req->getParameter("new_password");
    std::string validation = req->getParameter("pass_validation");
    //パスワードが確認と一致しているか確認
    if(password != validation){
        auto viewdate = HttpViewData();
        viewdate.insert("message", "パスワードが一致しません");
        callback(HttpResponse::newHttpViewResponse("NewUser.csp", viewdate));
        return;
    }
    try
    {
        //ランダム文字列saltを生成,passと繋げて暗号化ハッシュを作成
        char salt[LEN+1];
        srandom((unsigned int)time(NULL));
        for (int i = 0; i < LEN; i++)
        {
            salt[i] = getRandomChar();
        }
        salt[LEN] = '\0';
        std::cout << "salt:" << salt << std::endl;
        std::cout << "pass:" << password << std::endl;
        std::string passsalt = salt + password;
        std::cout << " salt+pass:"<<passsalt << std::endl;
        //ハッシュ化
        std::array<u_char, SHA512_DIGEST_LENGTH> passhash = {0};
        std::string digest = createDigestArray<char>(passsalt.c_str(), static_cast<size_t>(passsalt.length()), passhash);
        std::cout << digest << std::endl;
        //DBにsaltと共に登録
        auto DBclient = drogon::app().getDbClient("default");
        auto result = DBclient->execSqlAsyncFuture(
            AddUserSQL(digest,std::string(salt)),UserID).get();
        auto addstate = DBclient->execSqlAsyncFuture(
                                    u8"INSERT INTO state VALUES(0,'未着手',0,$1),(1,'完成',-1,$1),(2,'下書き',1,$1),(3,'ラフ',2,$1),(4,'線画',3,$1),(5,'キャラ塗り',4,$1),(6,'背景',5,$1)",UserID)
                            .get();
        auto viewdate = HttpViewData();
        viewdate.insert("message", "");
        callback(HttpResponse::newHttpViewResponse("LoginForm.csp", viewdate));
        return;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
}
std::string login::LoginSQL() const
{
    return std::string("SELECT * FROM illustrator where id = $1");
}
std::string login::AddUserSQL(std::string digest,std::string salt) const
{
    return std::string("INSERT INTO illustrator VALUES($1,'"+
    digest +"','" + salt
        +"')");
}
char login::getRandomChar(void) const
{
    const char charlist[] = 
    "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM1234567890";
    return charlist[random() % strlen(charlist)];
}
