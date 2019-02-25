#pragma once

#include "../menu/menu.hpp"
#include "../../dependencies/common_includes.hpp"

class antiaim : public singleton<antiaim>
{
public:
	void createmove(c_usercmd * cmd);
};