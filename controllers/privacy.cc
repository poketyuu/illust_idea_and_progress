#include "privacy.h"

void privacy::asyncHandleHttpRequest(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback)
{
    // write your application logic here
    callback(HttpResponse::newHttpViewResponse("privacy_policy.csp"));
}
