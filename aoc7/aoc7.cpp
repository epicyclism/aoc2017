#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <numeric>
#include <ranges>

#include <fmt/format.h>

#include "ctre_inc.h"
#include "timer.h"

// jptl (61)
//ugml (68) -> gyxo, ebii, jptl

auto get_input()
{
	std::map<std::string, std::vector<std::string>> al;
	std::map<std::string, int> wts;
	std::string ln;
	while(std::getline(std::cin, ln))
	{
		if(auto[m, n, w] = ctre::starts_with<"([a-z]+) .(\\d+).">(ln); m)
		{
			wts.insert(std::make_pair(n.to_string(), w.to_number<int>()));
			al.emplace(n.to_string(), std::vector<std::string>());
			if(m.size() != ln.size())
				for(auto e : ctre::search_all<"([a-z]+)">(std::string_view(ln.begin() + m.size(), ln.end())))
					al[n.to_string()].emplace_back(e.to_string());
		}
		else
			fmt::println("parse fail {}", ln);
	}
	return std::make_pair(al, wts);
}

void print_al(auto const& al)
{
	for(auto& v: al)
	{
		fmt::print("{} :", v.first);
		for(auto& e: v.second)
			fmt::print(" {}", e);
		fmt::println("");
	}
}

void print_wts(auto const& wts)
{
	fmt::println("wts size = {}", wts.size());
	for(auto p: wts)
		fmt::println("{} = {}", p.first, p.second);
}

std::string pt1(auto const& in)
{
	timer t("p1");
	std::map<std::string, std::vector<std::string>> const& al = in.first;

	std::map<std::string, int> ts;
	for(auto& v : al)
		ts.insert(std::make_pair(v.first, 0));
	for(auto& v: al)
		for(auto& e: v.second)
			++ts[e];
	for(auto& v: ts)
		if(v.second == 0)
		 return v.first;
	return "0000";
}

int calc_wt_above(std::map<std::string, std::vector<std::string>> const& al, 
					std::map<std::string, int> const& wts,
					std::string const& rt,
					std::map<std::string, int>& wt_above)
{
	auto wt = wts.at(rt);
	for(auto& e: al.at(rt))
		wt += calc_wt_above(al, wts, e, wt_above);
//	fmt::println("{} = {}", rt, wt);
	wt_above[rt] = wt;
	return wt;
}

bool is_balanced(std::map<std::string, std::vector<std::string>> const& al, 
					std::map<std::string, int> const& wt_above,
					std::string const& rt,
					std::string& s)
{
	int wt = -1;
	for(auto& e: al.at(rt))
	{
		if(wt == -1)
			wt = wt_above.at(e);
		else
		if(wt != wt_above.at(e))
		{
			s = rt;
			return false;
		}
	}
	return true;
}

void find_out_of_balance(std::map<std::string, std::vector<std::string>> const& al, 
					std::map<std::string, int> const& wt_above,
					std::string const& rt, std::string& s)
{
	if(!is_balanced(al, wt_above, rt, s))
	{
		for(auto& e: al.at(rt))
			find_out_of_balance(al, wt_above, e, s);
	}
}

int64_t pt2(auto const& in, std::string const& rt)
{
	timer t("p2");
	std::map<std::string, std::vector<std::string>> const& al = in.first;
	std::map<std::string, int> const& wts = in.second;

	std::map<std::string, int> wt_above;
	calc_wt_above(al, wts, rt, wt_above);
	std::string s;
	find_out_of_balance(al, wt_above, rt, s);
	std::vector<std::pair<int, int>> vv;
	for(auto& e: al.at(s))
		vv.emplace_back(wt_above.at(e), wts.at(e));
	std::ranges::sort(vv, [](auto& l, auto& r){ return l.first < r.first;});
	if(vv[0].first == vv[1].first)
		return vv.back().second - (vv.back().first - vv.front().first);
	else
		return vv.front().second - (vv.front().first - vv.back().first);
}

int main()
{
	auto in = get_input();
	auto p1 = pt1(in);
	auto p2 = pt2(in, p1);
	fmt::println("pt1 = {}", p1);
	fmt::println("pt2 = {}", p2);
}
