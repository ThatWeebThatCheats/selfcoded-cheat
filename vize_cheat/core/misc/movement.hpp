#pragma once

#include "../menu/menu.hpp"
#include "../../dependencies/common_includes.hpp"
#include "../../source-sdk/math/QAngle.hpp"

class misc : public singleton<misc>
{
public:
	void bhop(c_usercmd * cmd);
	void fastduck(c_usercmd * cmd);
	void autostrafer(c_usercmd * cmd);
	void fucknigga(c_usercmd * cmd);
};