#include <iostream>
#include <string>
#include <algorithm>
#include <numeric>
#include <ranges>

#include <fmt/format.h>

#include "timer.h"

auto get_input()
{
	std::string ln;
	std::getline(std::cin, ln);
	return ln;
}

std::string uncancel(std::string const& in)
{
	std::string rs;
	rs.reserve(in.size());
	for(size_t n = 0; n < in.size(); ++n)
		if(in[n] == '!')
			++n;
		else
			rs.push_back(in[n]);
	return rs;
}

auto ungarbage(std::string const& in)
{
	int gc = 0;
	std::string rs;
	rs.reserve(in.size());
	bool in_gbg = false;
	for(auto c: in)
	{
		if(in_gbg)
		{
			if( c == '>')
				in_gbg = false;
			else
				++gc;
		}
		else
		{
			if( c == '<')
				in_gbg = true;
			else
				rs.push_back(c);
		}
	}
	return std::make_pair(rs, gc);
}

auto pt12(auto const& in)
{
	timer t("p1");
	auto s = uncancel(in);
	auto[s2, gc] = ungarbage(s);
	int gn = 1;
	int scr = 0;
	for(auto c: s2)
	{
		if(c == '{')
		{
			scr += gn;
			++gn;
		}
		if(c == '}')
			--gn;
	}
	return std::make_pair(scr, gc);
}

int main()
{
	auto in = get_input();
	auto[ p1, p2] = pt12(in);
	fmt::println("pt1 = {}", p1);
	fmt::println("pt2 = {}", p2);
}
