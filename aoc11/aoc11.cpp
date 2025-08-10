#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <ranges>

#include <fmt/format.h>

#include "ctre_inc.h"
#include "timer.h"

enum class direct_t { n, ne, se, s, sw, nw, BOGUS};
direct_t from_v(std::string_view v)
{
	if(v == "n")
		return direct_t::n;
	if(v == "ne")
		return direct_t::ne;
	if(v == "se")
		return direct_t::se;
	if(v == "s")
		return direct_t::s;
	if(v == "sw")
		return direct_t::sw;
	if(v == "nw")
		return direct_t::nw;
	return direct_t::BOGUS;
}

auto get_input()
{
	std::vector<direct_t> r;
	std::string ln;
	std::getline(std::cin, ln);
	for(auto i: ctre::search_all<"([ensw]+)">(ln))
		r.emplace_back(from_v(i.to_view()));
	return r;
}

int steps(int x, int y)
{
	y = std::abs(y);
	x = std::abs(x);
	y -= x;
	return x + y / 2;
}

auto pt12(auto const& in)
{
	timer t("p12");
	int mx = 0;
	int x = 0;
	int y = 0;
	for(auto m: in)
	{
		switch(m)
		{
			case direct_t::n:
				y += 2;
				break;
			case direct_t::ne:
				++x;
				++y;
				break;
			case direct_t::se:
				++x;
				--y;
				break;
			case direct_t::s:
				y -= 2;
				break;
			case direct_t::sw:
				--x;
				--y;
				break;
			case direct_t::nw:
				--x;
				++y;
				break;
			case direct_t::BOGUS:
				fmt::println("BOGUS");
				break;
		}
		if(steps(x, y) > mx)
			mx = steps(x, y);
	}
	return std::make_pair(steps(x, y), mx);
}

int main()
{
	auto in = get_input();
	auto[p1, p2] = pt12(in);
	fmt::println("pt1 = {}", p1);
	fmt::println("pt2 = {}", p2);
}
