#pragma once

#include "SFML/System/Vector2.hpp"


//TOD::move to config, configuration for tanks types and weapons types/damage
namespace bt
{
	namespace game_entity_consts
	{
		static sf::Vector2f tank_size{33.0f, 54.0f};
		static sf::Vector2f bullet_size{ 6.0f, 24.0f };
		static float turret_length = 8.0f;//meters
		static sf::Vector2u game_field_size = sf::Vector2u{ 1024, 768 };

		static sf::Int32 tank_hp = 100;
		static sf::Int32 bullet_damage = 15;
	}

	namespace physics_consts
	{
		static float pixels_per_meters = 10.0f; // 1 meter = 10 pixels
		static b2Vec2 game_field_size = b2Vec2{ 102.4f, 76.8f };

		static float tank_speed = 50.0f; //meters per second
		static float bullet_speed = 500.0f; //meters per second
	}
}
