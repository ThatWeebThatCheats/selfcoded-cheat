#pragma once

#include "../menu/menu.hpp"
#include "../../dependencies/common_includes.hpp"

class visuals : public singleton<visuals>
{
public:
	void rendername();
	void renderarmor();
	void renderhealth();
	void renderesp();
};