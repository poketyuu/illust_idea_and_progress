/**
 *
 *  IDlogin.h
 *
 */

#pragma once

#include <drogon/HttpFilter.h>
using namespace drogon;


class IDlogin : public HttpFilter<IDlogin>
{
  public:
    IDlogin() {}
    void doFilter(const HttpRequestPtr &req,
                  FilterCallback &&fcb,
                  FilterChainCallback &&fccb) override;
};

