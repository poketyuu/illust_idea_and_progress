#include "idea.h"
#define ID "loginUser"

// Add definition of your processing function here
void idea::get(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    callback(HttpResponse::newHttpViewResponse("IdeaCsp.csp"));
}
