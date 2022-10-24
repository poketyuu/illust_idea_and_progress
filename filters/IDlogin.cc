/**
 *
 *  IDlogin.cc
 *
 */

#include "IDlogin.h"

using namespace drogon;

void IDlogin::doFilter(const HttpRequestPtr &req,
                         FilterCallback &&fcb,
                         FilterChainCallback &&fccb)
{
    //Edit your logic here
    if (req->session()->find("loginUser"))
    {
        //Passed
        fccb();
        return;
    }
    //Check failed
    auto viewdata = HttpViewData();
    viewdata.insert("newpage", "/login/");
    auto res = drogon::HttpResponse::newHttpViewResponse("PageTransition.csp",viewdata);
    fcb(res);
}
