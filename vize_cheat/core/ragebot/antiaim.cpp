#include "antiaim.hpp"

void Yaw(c_usercmd *cmd)
{
	if (menu::get()._get("msc_yaw") == 0)
		cmd->view_angles.y += 0.f;
	else if (menu::get()._get("msc_yaw") == 1)
		cmd->view_angles.y += 180.f;
}

void Pitch(c_usercmd *cmd)
{
	if (menu::get()._get("msc_pitch") == 0)
		cmd->view_angles.x += 0.f;
	else if (menu::get()._get("msc_pitch") == 1)
		cmd->view_angles.x += 89.f;
	else if (menu::get()._get("msc_pitch") == 2)
		cmd->view_angles.x -= 89.f;
}

void antiaim::createmove(c_usercmd *cmd)
{
	player_t *local = (player_t*)interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player());

	if (!local || !local->health() > 0)
		return;

	if (cmd->buttons & in_use || cmd->buttons & in_grenade1 || cmd->buttons & in_grenade2)
		return;

	weapon_t* weapon = local->active_weapon();

	if (!weapon)
		return;

	if (weapon->next_primary_attack() - interfaces::globals->cur_time < interfaces::globals->interval_per_tick && (cmd->buttons & in_attack || cmd->buttons & in_attack2))
		return;

	if (weapon->is_grenade() && weapon->throw_time() > 0.1f)
		return;

	Yaw(cmd);
	Pitch(cmd);

	if (menu::get()._get("msc_desync"))
	{
		// if (!bSendPacket)
			// cmd->view_angles.y += interfaces::clientstate->choked_commands == 0 ? -59 : 180;
	}
}