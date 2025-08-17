#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <algorithm>
#include <numeric>
#include <ranges>

#include <fmt/format.h>

#include "ctre_inc.h"
#include "timer.h"

using v3 = std::array<int64_t, 3>;

int md(v3 const& v)
{
	return std::abs(v[0]) + std::abs(v[1]) + std::abs(v[2]);
}

struct particle
{
	v3 p_;
	v3 v_;
	v3 a_;
	int64_t to_o_;
	int64_t to_op_;
	particle(int px, int py, int pz, int vx, int vy, int vz, int ax, int ay, int az)
	{
		p_ = {px, py, pz};
		v_ = {vx, vy, vz};
		a_ = {ax, ay, az};
		to_o_ = md(p_);
		to_op_ = to_o_ + 1;
	}
	void update()
	{
		std::ranges::transform(v_, a_, v_.begin(), [](auto a, auto b){ return b + a;});
		std::ranges::transform(p_, v_, p_.begin(), [](auto a, auto b){ return b + a;});
		to_op_ = to_o_;
		to_o_ = md(p_);
	}
	bool receding() const
	{
		return to_o_ > to_op_;
	}
	int velocity() const
	{
		return md(v_);
	}
};

auto get_input()
{
	std::vector<particle> v;
	std::string ln;
	while(std::getline(std::cin, ln))
	{
		// p=<3,0,0>, v=<2,0,0>, a=<-1,0,0>
		if(auto[m, px, py, pz, vx, vy, vz, ax, ay, az] = ctre::match<"p=<(-?\\d+),(-?\\d+),(-?\\d+)>, v=<(-?\\d+),(-?\\d+),(-?\\d+)>, a=<(-?\\d+),(-?\\d+),(-?\\d+)>">(ln); m)
			v.emplace_back(px.to_number<int>(), py.to_number<int>(), pz.to_number<int>(),
				vx.to_number<int>(), vy.to_number<int>(), vz.to_number<int>(),
				ax.to_number<int>(), ay.to_number<int>(), az.to_number<int>());
		else
			fmt::println("Parsse fail : {}", ln);
	}
	return v;
}

int64_t pt1(auto in)
{
	timer t("p1");
	while(!std::ranges::all_of(in, &particle::receding))
		std::ranges::for_each(in, &particle::update);
	for(int x = 0; x < 10; ++x)
	{
		for(int n = 0; n < 100; ++n)
			std::ranges::for_each(in, &particle::update);
//		fmt::println("{}", std::distance(in.begin(), std::ranges::min_element(in, std::less<>(), &particle::to_o_)));
	}
	return std::distance(in.begin(), std::ranges::min_element(in, std::less<>(), &particle::to_o_));
}

void eliminate_collisions(std::vector<particle>& in)
{
	std::ranges::sort(in, [](auto& l, auto& r){
		if(l.p_[0] == r.p_[0])
		{
			if(l.p_[1] == r.p_[1])
				return l.p_[2] < r.p_[2];
			return l.p_[1] < r.p_[1];
		}
		return l.p_[0] < r.p_[0];
	});
	while(1)
	{
		auto ib = std::ranges::adjacent_find(in, [](auto& l, auto& r){
			return l.p_[0] == r.p_[0] &&
					l.p_[1] == r.p_[1] &&
					l.p_[2] == r.p_[2];
		});
		if( ib == in.end())
			return;
		auto er = std::equal_range(ib, in.end(), *ib, [](auto& l, auto& r){
			if(l.p_[0] == r.p_[0])
			{
				if(l.p_[1] == r.p_[1])
					return l.p_[2] < r.p_[2];
				return l.p_[1] < r.p_[1];
			}
			return l.p_[0] < r.p_[0];
		});
		in.erase(er.first, er.second);
	}
}

int64_t pt2(auto in)
{
	timer t("p2");
	while(!std::ranges::all_of(in, &particle::receding))
	{
		eliminate_collisions(in);
		std::ranges::for_each(in, &particle::update);
	}
	return in.size();
}

int main()
{
	auto in = get_input();
	auto p1 = pt1(in);
	auto p2 = pt2(in);
	fmt::println("pt1 = {}", p1);
	fmt::println("pt2 = {}", p2);
}
