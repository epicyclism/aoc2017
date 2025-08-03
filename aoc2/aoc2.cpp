#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <ranges>

#include <fmt/format.h>

#include "ctre_inc.h"
#include "timer.h"

auto get_input()
{
	std::vector<std::vector<int>> rv;
	std::string ln;
	while(std::getline(std::cin, ln))
	{
		rv.emplace_back();
		for(auto[m, d]: ctre::search_all<"(\\d+)">(ln))
			rv.back().emplace_back(d.to_number<int>());
	}
	return rv;
}

int pt1(auto const& in)
{
	timer t("p1");
	int cnt = 0;
	for(auto& r: in)
	{
		auto[mn, mx] = std::ranges::minmax_element(r);
		cnt += *mx - *mn;
	}
	return cnt;
}

int pt2(auto const& in)
{
	timer t("p2");
	auto wkr = [](auto& r) -> int
	{
		for(int a = 0; a < r.size() - 1; ++a)
			for(int b = a + 1; b < r.size(); ++b)
			{
				auto aa = r[a];
				auto bb = r[b];
				if (aa > bb && aa % bb == 0)
					return aa / bb;
				if( bb > aa && bb % aa == 0)
					return bb / aa;
			}
			return 0;
	};
	int cnt = 0;
	for(auto& r: in)
		cnt += wkr(r) ;
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
