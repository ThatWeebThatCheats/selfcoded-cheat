#include "movement.hpp"

void misc::bhop(c_usercmd *cmd)
{
	player_t *local = (player_t*)interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player());

	if (!(local->flags() & fl_onground))
		cmd->buttons |= in_jump;
}

void misc::fastduck(c_usercmd *cmd)
{
	cmd->buttons |= in_bullrush;
}

template<class T, class U>
T clamp(T in, U low, U high)
{
	if (in <= low)
		return low;

	if (in >= high)
		return high;

	return in;
}

void misc::autostrafer(c_usercmd *cmd)
{
	player_t *local = (player_t*)interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player());

	if (!local)
		return;

	if (!local->health() > 0)
		return;

	if (!(local->flags() & fl_onground))
	{
		if (cmd->mouse_dx > 1 || cmd->mouse_dx < -1)
		{
			cmd->side_move = clamp(cmd->mouse_dx < 0.f ? -400.f : 400.f, -400, 400);
		}
		else
		{
			if (local->velocity().length_sqr() == 0 || local->velocity().length_sqr() == NAN || local->velocity().length_sqr() == INFINITE)
			{
				cmd->forward_move = 400;
				return;
			}

			cmd->forward_move = clamp(5850.f / local->velocity().length_sqr(), -400, 400);

			if (cmd->forward_move < -400 || cmd->forward_move > 400)
				cmd->forward_move = 0;

			cmd->side_move = clamp((cmd->command_number % 2) == 0 ? -400.f : 400.f, -400, 400);

			if (cmd->side_move < -400 || cmd->side_move > 400)
				cmd->side_move = 0;
		}
	}
}

void misc::fucknigga(c_usercmd *cmd)
{
	player_t *local = (player_t*)interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player());

	if (menu::get()._get("msc_slowwalk_amt") == 0 || !GetAsyncKeyState(VK_SHIFT))
		return;

	auto weapon_handle = local->active_weapon();

	if (!weapon_handle)
		return;

	float amt = 0.0034f * menu::get()._get("msc_slowwalk_amt");

	float speed = local->velocity().length_sqr();

	vec2_t forward;

	vec2_t src = forward * -speed;

	if (speed >= (weapon_handle->get_weapon_data()->max_speed * amt))
	{
		cmd->forward_move = src.x;
		cmd->side_move = src.y;
	}
}