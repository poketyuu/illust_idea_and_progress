//this file is generated by program(drogon_ctl) automatically,don't modify it!
#include "temphead.h"
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
std::string temphead::genText(const DrTemplateData& temphead_view_data)
{
	drogon::OStringStream temphead_tmp_stream;
	std::string layoutName{""};
	temphead_tmp_stream << "<html lang=\"ja\">\n";
	temphead_tmp_stream << "<head>\n";
	temphead_tmp_stream << "<meta charset=\"UTF-8\">\n";
	temphead_tmp_stream << "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n";
	temphead_tmp_stream << "<link rel=\"stylesheet\" href=\"https://cdn.jsdelivr.net/npm/bootstrap@5.1.3/dist/css/bootstrap.min.css\" integrity=\"sha384-1BmE4kWBq78iYhFldvKuhfTAU6auU8tT94WrHftjDbrCEXSU1oBoqyl2QvZ6jIW3\" crossorigin=\"anonymous\"></link>\n";
	temphead_tmp_stream << "<style>\n";
	temphead_tmp_stream << "body {padding-top: 60px;}\n";
	temphead_tmp_stream << "</style>\n";
	temphead_tmp_stream << "</head>\n";
if(layoutName.empty())
{
std::string ret{std::move(temphead_tmp_stream.str())};
return ret;
}else
{
auto templ = DrTemplateBase::newTemplate(layoutName);
if(!templ) return "";
HttpViewData data = temphead_view_data;
auto str = std::move(temphead_tmp_stream.str());
if(!str.empty() && str[str.length()-1] == '\n') str.resize(str.length()-1);
data[""] = std::move(str);
return templ->genText(data);
}
}
