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

enum test_t { LT, GT, LTE, GTE, EQ, NE, TILT};

test_t from_v(std::string_view v)
{
	if(v == "<")
		return LT;
	if(v == ">")
		return GT;
	if(v == "<=")
		return LTE;
	if(v == ">=")
		return GTE;
	if(v == "==")
		return EQ;
	if(v == "!=")
		return NE;
	return TILT;
}

struct inst_t
{
	std::string tgt_;
	int         amt_;
	std::string cond_;
	test_t		ct_;
	int			cv_;
};

auto get_input()
{
	std::vector<inst_t> r;
	std::string ln;
	while(std::getline(std::cin, ln))
	{
		if(auto[m, t, v, cr, c, cv] = ctre::match<"([a-z]+) inc (-?\\d+) if ([a-z]+) ([><=!]+) (-?\\d+)">(ln); m)
		{
			r.emplace_back(t.to_string(), v.to_number<int>(), cr.to_string(), from_v(c.to_view()), cv.to_number<int>());
		}
		else
		if(auto[m, t, v, cr, c, cv] = ctre::match<"([a-z]+) dec (-?\\d+) if ([a-z]+) ([><=!]+) (-?\\d+)">(ln); m)
		{
			r.emplace_back(t.to_string(), -v.to_number<int>(), cr.to_string(), from_v(c.to_view()), cv.to_number<int>());
		}
		else
			fmt::println("parse fail {}", ln);	
	}
	return r;
}

bool do_condition(test_t tt, int a, int b)
{
	switch(tt)
	{
		case LT:
			return a < b;
		case GT:
			return a > b;
		case LTE:
			return a <= b;
		case GTE:
			return a >= b;
		case EQ:
			return a == b;
		case NE:
			return a != b;
		default:
		case TILT:
			break;
	}
	return false;
}

auto pt12(auto const& in)
{
	timer t("p1");
	std::map<std::string, int> rr;
	int mx = 0;
	for(auto& i: in)
	{
		auto cv = rr[i.cond_];
		if(do_condition(i.ct_, cv, i.cv_))
		{
			int v = rr[i.tgt_] + i.amt_;
			if( v > mx)
				mx = v;
			rr[i.tgt_] = v;
		}
	}

	return std::make_pair((*std::ranges::max_element(rr, [](auto& l, auto& r){ return l.second < r.second;})).second, mx);
//	return *std::ranges::max_element(rr, &std::pair<const std::string, int>::second);
}

int main()
{
	auto in = get_input();
	auto[p1, p2] = pt12(in);
	fmt::println("pt1 = {}", p1);
	fmt::println("pt2 = {}", p2);
}
