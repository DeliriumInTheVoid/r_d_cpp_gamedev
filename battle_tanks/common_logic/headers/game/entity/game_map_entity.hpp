#pragma once

#include <box2d/box2d.h>

#include "game_object_type.hpp"


class game_map_entity : public bt::game_object_entity
{
public:
    explicit game_map_entity(const sf::Uint32 id, const b2Vec2& game_field_size, const std::weak_ptr<bt::physics_body_factory>& ph_body_factory)
        : game_object_entity(id, bt::game_object_type::map_forest), game_field_size_{game_field_size}, ph_body_factory_{ph_body_factory}
    {
    }

    virtual ~game_map_entity() override
    {
        if (!ph_body_factory_.expired())
        {
            const auto factory = ph_body_factory_.lock();
            for (auto* phy_body : phy_bodies_)
            {
                factory->destroy_body(phy_body);
            }

            phy_bodies_.clear();
        }
    }

public:
    void build_map()
    {
        if (ph_body_factory_.expired())
        {
            return;
        }

        const auto phy_body_factory = ph_body_factory_.lock();
        build_edges(phy_body_factory);
    }

    void create_rock(const b2Vec2 position, const b2Vec2 size)
    {
        if (ph_body_factory_.expired())
        {
            return;
        }
        const auto phy_body_factory = ph_body_factory_.lock();

        b2BodyDef rock_body_def;
        rock_body_def.type = b2_staticBody;
        rock_body_def.position.Set(position.x, position.y);
        const auto rock_body = phy_body_factory->create_body(rock_body_def);
        phy_bodies_.push_back(rock_body);

        b2PolygonShape rock_shape;
        rock_shape.SetAsBox(size.x /2.0f, size.y / 2.0f);
        auto* fixture = rock_body->CreateFixture(&rock_shape, 1.0f);
        b2Filter filter;
        filter.categoryBits = 0x0002;
        filter.maskBits = 0x8006;

        fixture->SetFilterData(filter);
    }

public:
    void restore_frame(const bt::game_object_frame_restorer& restorer) override
    {
    }

    void update(const float delta_time) override
    {
    }

    virtual bool is_out_of_edges(const b2Vec2& size) const override
    {
        return false;
    }

protected:

    //TODO:: create map from server configuration
    [[nodiscard]] std::shared_ptr<game_object_frame> create_frame() const override
    {
        return nullptr;
    }
    void fill_frame_data(const std::shared_ptr<game_object_frame>& object_frame) const override
	{}

private:
    void build_edges(const std::shared_ptr<bt::physics_body_factory>& factory)
    {
        b2BodyDef edge_body_def;
        edge_body_def.type = b2_staticBody;
        b2Body* edges_body = factory->create_body(edge_body_def);
        phy_bodies_.push_back(edges_body);

        b2Filter edge_filter;
        edge_filter.categoryBits = 0x8000;
        edge_filter.maskBits = 0x0007;

        b2EdgeShape top_edge_shape;
        top_edge_shape.SetTwoSided(b2Vec2(0.0f, 0.0f), b2Vec2(game_field_size_.x, 0.0f));
        auto* edge_fixture = edges_body->CreateFixture(&top_edge_shape, 1.0f);
        edge_fixture->SetFilterData(edge_filter);

        b2EdgeShape bottom_edge_shape;
        bottom_edge_shape.SetTwoSided(b2Vec2(0.0f, game_field_size_.y), game_field_size_);
        edge_fixture = edges_body->CreateFixture(&bottom_edge_shape, 1.0f);
        edge_fixture->SetFilterData(edge_filter);

        b2EdgeShape left_edge_shape;
        left_edge_shape.SetTwoSided(b2Vec2(0.0f, 0.0f), b2Vec2(0.0f, game_field_size_.y));
        edge_fixture = edges_body->CreateFixture(&left_edge_shape, 1.0f);
        edge_fixture->SetFilterData(edge_filter);

        b2EdgeShape right_edge_shape;
        right_edge_shape.SetTwoSided(b2Vec2(game_field_size_.x, 0.0f), game_field_size_);
        edge_fixture = edges_body->CreateFixture(&right_edge_shape, 1.0f);
        edge_fixture->SetFilterData(edge_filter);
    }

private:
    b2Vec2 game_field_size_;
    std::weak_ptr<bt::physics_body_factory> ph_body_factory_;
    std::vector<b2Body*> phy_bodies_{};
};
