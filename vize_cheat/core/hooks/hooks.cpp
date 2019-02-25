#pragma once
#include "../menu/menu.hpp"
#include "../misc/movement.hpp"
#include "../ragebot/antiaim.hpp"
#include "../visuals/visuals.h"
#include "../../dependencies/common_includes.hpp"
#include <algorithm>

std::unique_ptr<vmt_hook> hooks::client_hook;
std::unique_ptr<vmt_hook> hooks::clientmode_hook;
std::unique_ptr<vmt_hook> hooks::panel_hook;
std::unique_ptr<vmt_hook> hooks::renderview_hook;
WNDPROC hooks::wndproc_original = NULL;

void hooks::initialize( ) 
{
	client_hook = std::make_unique<vmt_hook>( );
	clientmode_hook = std::make_unique<vmt_hook>( );
	panel_hook = std::make_unique<vmt_hook>( );
	renderview_hook = std::make_unique<vmt_hook>( );

	client_hook->setup( interfaces::client );
	client_hook->hook_index( 37, reinterpret_cast< void* >( frame_stage_notify ) );

	clientmode_hook->setup( interfaces::clientmode );
	clientmode_hook->hook_index( 24, reinterpret_cast< void* >( create_move ) );

	panel_hook->setup( interfaces::panel );
	panel_hook->hook_index( 41, reinterpret_cast< void* >( paint_traverse ) );

	renderview_hook->setup( interfaces::render_view );
	renderview_hook->hook_index( 9, reinterpret_cast< void* >( scene_end ) );

	wndproc_original = ( WNDPROC ) SetWindowLongPtrA( FindWindow( "Valve001", NULL ), GWL_WNDPROC, ( LONG ) wndproc );

	interfaces::console->get_convar( "mat_queue_mode" )->set_value( 0 );
	interfaces::console->get_convar( "viewmodel_fov" )->callbacks.SetSize( 0 );
	interfaces::console->get_convar( "mat_postprocess_enable" )->set_value( 0 );
	interfaces::console->get_convar( "crosshair" )->set_value( 1 );
	render::get( ).setup_fonts( );

	interfaces::engine->execute_cmd("clear");

	interfaces::engine->execute_cmd("echo Welcome To vize's cheat");
}

void hooks::shutdown( ) 
{
	clientmode_hook->release( );
	client_hook->release( );
	panel_hook->release( );
	renderview_hook->release( );

	SetWindowLongPtrA( FindWindow( "Valve001", NULL ), GWL_WNDPROC, ( LONG ) wndproc_original );
}

bool __stdcall hooks::create_move( float frame_time, c_usercmd* user_cmd )
{
	static auto original_fn = reinterpret_cast< create_move_fn >( clientmode_hook->get_original( 24 ) )( interfaces::clientmode, frame_time, user_cmd );

	if (!user_cmd || !user_cmd->command_number)
		return original_fn;

	if (!interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()))
		return original_fn;

	menu::get().tick();
	if (menu::get()._get("msc_bhop"))
		misc::get().bhop(user_cmd);

	menu::get().tick();
	if (menu::get()._get("msc_fastduck"))
		misc::get().fastduck(user_cmd);

	menu::get().tick();
	if (menu::get()._get("msc_autostrafer"))
		misc::get().autostrafer(user_cmd);

	menu::get().tick();
	if (menu::get()._get("msc_antiaim"))
		antiaim::get().createmove(user_cmd);

	return false;
}

void __stdcall hooks::frame_stage_notify( int frame_stage )
{
	reinterpret_cast< frame_stage_notify_fn >( client_hook->get_original( 37 ) )( interfaces::client, frame_stage );

	if (menu::get()._get("msc_viewmodel"))
	{
		auto x = interfaces::console->get_convar("viewmodel_offset_x");
		auto y = interfaces::console->get_convar("viewmodel_offset_y");
		auto z = interfaces::console->get_convar("viewmodel_offset_z");

		x->set_value(menu::get()._get("msc_viewmodel_x"));
		y->set_value(menu::get()._get("msc_viewmodel_y"));
		z->set_value(menu::get()._get("msc_viewmodel_z"));
	}
	else
	{

	}
}

void __stdcall hooks::paint_traverse( unsigned int panel, bool force_repaint, bool allow_force ) 
{
	std::string panel_name = interfaces::panel->get_panel_name( panel );

	reinterpret_cast< paint_traverse_fn >( panel_hook->get_original( 41 ) )( interfaces::panel, panel, force_repaint, allow_force );

	static unsigned int _panel = 0;
	static bool once = false;

	if ( !once )
	{
		PCHAR panel_char = ( PCHAR ) interfaces::panel->get_panel_name( panel );

		if ( strstr( panel_char, "MatSystemTopPanel" ) )
		{
			_panel = panel;
			once = true;
		}
	}
	else if ( _panel == panel ) 
	{
		menu::get( ).tick( );

		if (menu::get()._get("msc_esp"))
			visuals::get().renderesp();

		if (menu::get()._get("msc_watermark")) 
		{
			render::get().draw_filled_rect(5, 5, 60, 20, color(0, 0, 0, 255));
			render::get().draw_text(5, 5, render::get().main_font, "vize_cheat", false, color(255, 255, 255));
		}
	}
}

void __stdcall hooks::scene_end( )
{
	reinterpret_cast< scene_end_fn >( renderview_hook->get_original( 9 ) )( interfaces::render_view );
}

LRESULT __stdcall hooks::wndproc( HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam ) 
{
	return CallWindowProcA( wndproc_original, hwnd, message, wparam, lparam );
}
