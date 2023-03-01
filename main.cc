#include <drogon/drogon.h> 
#include <chrono>
int main() {
    //Set HTTP listener address and port
    drogon::app().addListener("0.0.0.0",8080);
    //Load config file
    drogon::app().loadConfigFile("../config.json");
    //Run HTTP framework,the method will block in the internal event loop
    drogon::app().enableSession(3600).run();
    return 0;
}
