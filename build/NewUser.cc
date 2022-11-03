//this file is generated by program(drogon_ctl) automatically,don't modify it!
#include "NewUser.h"
#include <drogon/utils/OStringStream.h>
#include <string>
#include <map>
#include <vector>
#include <set>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <list>
#include <deque>
#include <queue>
using namespace drogon;
std::string NewUser::genText(const DrTemplateData& NewUser_view_data)
{
	drogon::OStringStream NewUser_tmp_stream;
	std::string layoutName{""};
	NewUser_tmp_stream << "<!DOCTYPE HTML>\n";
{
    auto templ=DrTemplateBase::newTemplate("temphead");
    if(templ){
      NewUser_tmp_stream<< templ->genText(NewUser_view_data);
    }
}
NewUser_tmp_stream<<"\n";
	NewUser_tmp_stream << "<body class=\"text-center\">\n";
	NewUser_tmp_stream << "<main>\n";
	NewUser_tmp_stream << "<h1>新規ユーザー登録</h1>\n";
	NewUser_tmp_stream << "<form action=\"newUser\" method=\"POST\">\n";
	NewUser_tmp_stream << "<div class=\"form-floating\">\n";
	NewUser_tmp_stream << "<div class=\"mb-3\">\n";
	NewUser_tmp_stream << "<label>ログインID:</label>\n";
	NewUser_tmp_stream << "<input type=\"text\" name=\"new_id\" required>\n";
	NewUser_tmp_stream << "</div>\n";
	NewUser_tmp_stream << "<div class=\"mb-3\">\n";
	NewUser_tmp_stream << "<label>パスワード:</label>\n";
	NewUser_tmp_stream << "<input type=\"password\" name=\"new_password\" required>\n";
	NewUser_tmp_stream << "</div>\n";
	NewUser_tmp_stream << "<div class=\"mb-3\">\n";
	NewUser_tmp_stream << "<label>パスワード(再入力):</label>\n";
	NewUser_tmp_stream << "<input type=\"password\" name=\"pass_validation\" required>\n";
	NewUser_tmp_stream << "</div>\n";
	NewUser_tmp_stream << "<div class=\"mb-3\">\n";
	NewUser_tmp_stream << "<label class=\"text-danger\">";
{
    auto & val=NewUser_view_data["message"];
    if(val.type()==typeid(const char *)){
        NewUser_tmp_stream<<*any_cast<const char *>(&val);
    }else if(val.type()==typeid(std::string)||val.type()==typeid(const std::string)){
        NewUser_tmp_stream<<*any_cast<const std::string>(&val);
    }
}
	NewUser_tmp_stream << "</label>\n";
	NewUser_tmp_stream << "</div>\n";
	NewUser_tmp_stream << "<button class=\"btn btn-lg btn-primary\" type=\"submit\">登録</button>\n";
	NewUser_tmp_stream << "</div>\n";
	NewUser_tmp_stream << "</form>\n";
	NewUser_tmp_stream << "</main>\n";
	NewUser_tmp_stream << "<script src=\"https://cdn.jsdelivr.net/npm/bootstrap@5.1.3/dist/js/bootstrap.bundle.min.js\" integrity=\"sha384-ka7Sk0Gln4gmtz2MlQnikT1wXgYsOg+OMhuP+IlRH9sENBO0LRn5q+8nbTov4+1p\" crossorigin=\"anonymous\"></script>\n";
	NewUser_tmp_stream << "</body>\n";
	NewUser_tmp_stream << "</html>\n";
if(layoutName.empty())
{
std::string ret{std::move(NewUser_tmp_stream.str())};
return ret;
}else
{
auto templ = DrTemplateBase::newTemplate(layoutName);
if(!templ) return "";
HttpViewData data = NewUser_view_data;
auto str = std::move(NewUser_tmp_stream.str());
if(!str.empty() && str[str.length()-1] == '\n') str.resize(str.length()-1);
data[""] = std::move(str);
return templ->genText(data);
}
}