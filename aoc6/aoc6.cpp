#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <algorithm>

#include <fmt/format.h>

#include "ctre_inc.h"
#include "timer.h"

auto get_input()
{
	std::vector<int> r;
	std::string ln;
	std::getline(std::cin, ln);
	for(auto v: ctre::search_all<"(\\d+)">(ln))
		r.emplace_back(v.to_number<int>());
	return r;
}

int pt1(auto in)
{
	timer t("p1");
	const int msk = in.size() - 1;
	std::set<std::vector<int>> ss;
	while(1)
	{
		if(ss.contains(in))
			return ss.size();
		ss.insert(in);
		size_t mx = std::distance(in.begin(), std::ranges::max_element(in));
		auto redist = in[mx];
		in[mx] = 0;
		while(redist)
		{
			++mx;
			mx &= msk;
			++in[mx];
			--redist;
		}
	}
	// not reached...
	return 0;
}

int pt2(auto in)
{
	timer t("p2");
	const int msk = in.size() - 1;
	std::map<std::vector<int>, size_t> mm;
	while(1)
	{
		if(mm.contains(in))
			return mm.size() - mm[in];
		mm[in] = mm.size();
		size_t mx = std::distance(in.begin(), std::ranges::max_element(in));
		auto redist = in[mx];
		in[mx] = 0;
		while(redist)
		{
			++mx;
			mx &= msk;
			++in[mx];
			--redist;
		}
	}
	// not reached...
	return 0;

}

int main()
{
	auto in = get_input();
	auto p1 = pt1(in);
	auto p2 = pt2(in);
	fmt::println("pt1 = {}", p1);
	fmt::println("pt2 = {}", p2);
}
