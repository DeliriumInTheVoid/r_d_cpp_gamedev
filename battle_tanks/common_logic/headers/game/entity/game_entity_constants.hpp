#pragma once

#include "SFML/System/Vector2.hpp"


namespace bt
{
	namespace game_entity_consts
	{
		static sf::Vector2f tank_size{33.0f, 54.0f};
		static sf::Vector2f bullet_size{ 6.0f, 24.0f };
		static float turret_length = 8.0f;//meters
	}

	namespace physics_consts
	{
		static float pixels_per_meters = 10.0f;
	}
}
