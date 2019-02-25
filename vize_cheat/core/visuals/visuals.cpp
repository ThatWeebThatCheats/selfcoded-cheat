#include "visuals.h"

struct
{
	player_t* pl;
	bool          is_enemy;
	bool          is_team;
	bool          is_visible;
	color         esp_color;
	color         clr2;
	color         textClr;
	color         flashClr;
	color         healthbar_color;
	color         hpClr2;
	color         armClr;
	color         armClr2;
	color         skeletClr;
	vec2_t        head_pos;
	vec2_t        feet_pos;
	RECT          bbox;
} esp_ctx;

void visuals::rendername()
{
	wchar_t buf[128];

	static int iterator;

	player_info_t pInfo;

	interfaces::engine->get_player_info(iterator, &pInfo);
}

void visuals::renderarmor()
{
	auto armour = esp_ctx.pl->armor();
}

void visuals::renderhealth()
{
	auto health = esp_ctx.pl->health();
}

void visuals::renderesp()
{
	player_t *local = (player_t*)interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player());

	for (auto i = -1; i <= interfaces::globals->max_clients; ++i)
	{
		player_t* entity = (player_t*)interfaces::entity_list->get_client_entity(i);

		if (!entity)
			continue;

		if (entity == local)
			continue;

		if (i < 65 && !entity->dormant() && entity->health() > 0)
		{
			menu::get().tick();

			if (menu::get()._get("msc_esp"))
			{
				menu::get().tick();

				if (menu::get()._get("msc_radar"))
					entity->spotted() = true;

				if (menu::get()._get("msc_names"))
					visuals::get().rendername();

				if (menu::get()._get("msc_health"))
					visuals::get().renderhealth();

				if (menu::get()._get("msc_armor"))
					visuals::get().renderarmor();
			}
		}
	}
}