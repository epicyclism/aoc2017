#include <iostream>
#include <string>
#include <vector>
#include <set>

#include <algorithm>

#include <fmt/format.h>

#include "ctre_inc.h"
#include "timer.h"

auto get_input()
{
	std::vector<std::string> rv;
	std::string ln;
	while(std::getline(std::cin, ln))
		rv.emplace_back(ln);
	return rv;
}

int pt1(auto const& in)
{
	timer t("p1");
	int cnt = 0;
	for(auto& r : in)
	{
		std::set<std::string> ss;
		for(auto w : ctre::search_all<"([a-z]+)">(r))
		{
			if(ss.contains(w.to_string()))
				goto nxt;
			ss.insert(w.to_string());
		}
		++cnt;
nxt:
	}
	return cnt;
}

int pt2(auto const& in)
{
	timer t("p2");
	int cnt = 0;
	for(auto& r : in)
	{
		std::set<std::string> ss;
		for(auto w : ctre::search_all<"([a-z]+)">(r))
		{
			std::string tmp(w.to_string());
			std::ranges::sort(tmp);
			if(ss.contains(tmp))
				goto nxt;
			ss.insert(tmp);
		}
		++cnt;
nxt:
	}
	return cnt;
}

int main()
{
	auto in = get_input();
	auto p1 = pt1(in);
	auto p2 = pt2(in);
	fmt::println("pt1 = {}", p1);
	fmt::println("pt2 = {}", p2);
}
