#include <iostream>
#include <string>
#include <vector>

#include <fmt/format.h>
#include <fmt/ranges.h>

#include "ctre_inc.h"
#include "timer.h"

struct state_t
{
	struct act_t
	{
		bool   write_;
		bool   move_left_;
		int8_t next_;
	};
	act_t acts_[2];
};

struct machine_t
{
	int8_t state_;
	int    steps_;
	std::vector<state_t> states_;
	int run()
	{
		constexpr size_t infinity = 4096;
		std::bitset<2*infinity> tape;
		size_t pos = infinity;
		for(int n = 0; n < steps_; ++n)
		{
			if(!tape[pos])
			{
				tape[pos] = states_[state_].acts_[0].write_;
				pos += (states_[state_].acts_[0].move_left_) ? -1 : 1;
				state_ = states_[state_].acts_[0].next_;
			}
			else
			{
				tape[pos] = states_[state_].acts_[1].write_;
				pos += (states_[state_].acts_[1].move_left_) ? -1 : 1;
				state_ = states_[state_].acts_[1].next_;
			}
		}
		return tape.count();
	}
};

bool read_state(machine_t& mach)
{
	std::string ln;
	if(!std::getline(std::cin, ln))
		return false;
	mach.states_.push_back(state_t());
	std::getline(std::cin, ln);
	std::getline(std::cin, ln);
	std::getline(std::cin, ln);
	if(auto[m, v] = ctre::match<"    - Write the value (\\d).">(ln); m)
		mach.states_.back().acts_[0].write_ = v.view()[0] == '1';
	else
		return false;
	std::getline(std::cin, ln);
	if(auto[m, lr] = ctre::match<"    - Move one slot to the (left|right).">(ln); m)
		mach.states_.back().acts_[0].move_left_ = lr.view()[0] == 'l';
	else
		return false;
	std::getline(std::cin, ln);
	if(auto[m, ns] = ctre::match<"    - Continue with state ([A-Z]).">(ln); m)
		mach.states_.back().acts_[0].next_ = ns.view()[0] - 'A';
	else
		return false;
	std::getline(std::cin, ln);
	std::getline(std::cin, ln);
	if(auto[m, v] = ctre::match<"    - Write the value (\\d).">(ln); m)
		mach.states_.back().acts_[1].write_ = v.view()[0] == '1';
	else
		return false;
	std::getline(std::cin, ln);
	if(auto[m, lr] = ctre::match<"    - Move one slot to the (left|right).">(ln); m)
		mach.states_.back().acts_[1].move_left_ = lr.view()[0] == 'l';
	else
		return false;
	std::getline(std::cin, ln);
	if(auto[m, ns] = ctre::match<"    - Continue with state ([A-Z]).">(ln); m)
		mach.states_.back().acts_[1].next_ = ns.view()[0] - 'A';
	else
		return false;

	return true;
}

auto get_input()
{
	machine_t mach;
	std::string ln;
	std::getline(std::cin, ln); 
	if(auto[m, s] = ctre::match<"Begin in state ([A-Z]).">(ln); m)
		mach.state_ = s.view()[0] - 'A';
	else
		fmt::println("parse fail >{}<", ln);
	std::getline(std::cin, ln); 
	if(auto[m, n] = ctre::match<"Perform a diagnostic checksum after (\\d+) steps.">(ln); m)
		mach.steps_ = n.to_number<int>();
	else
		fmt::println("parse fail {}", ln);
	while(read_state(mach))
		;
	return mach;
}


int pt1(auto& in)
{
	timer t("p1");
	return in.run();
}

int main()
{
	auto in = get_input();
	auto p1 = pt1(in);
	fmt::println("pt1 = {}", p1);
}
