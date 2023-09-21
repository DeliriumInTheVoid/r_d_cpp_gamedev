#include <cmath>
#include <numbers>
#include <chrono>

#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>

#include "boost/di.hpp"

#include "debug_draw.hpp"

typedef long long unsigned uuid;
static uuid current_uuid{ 1 };
uuid generate_uuid()
{
    return current_uuid++;
}

float deg_to_rad(float rotation)
{
    return static_cast<float>(std::numbers::pi * static_cast<double>(rotation) / 180.0);
}

class container;
class render_object;

enum class move_action
{
    stop = 0,
    forward,
    backward,
};

enum rotate_action
{
    stop = 0,
    left,
    right,
};

enum class game_object_type : unsigned
{
    unknown = 0,
    tank,
    bullet,
    wall,
    brick
};

struct game_object_b2d_link
{
	game_object_type type{ game_object_type::unknown };
	uuid id{ 0 };
};

class game_object
{
public:
    explicit game_object(uuid id) : id_{ id }
    {
    }

    virtual ~game_object() = default;

public:
    virtual void initialize() = 0;
    virtual void update(float delta_time) = 0;
    virtual void free() = 0;

public:
    const std::shared_ptr<render_object>& get_render_object() const
    {
        return render_object_;
    }

    const b2Body* get_physics_body() const
    {
        return physics_body_;
    }

protected:
    std::shared_ptr<render_object> render_object_{ nullptr };
    b2Body* physics_body_{ nullptr };
    uuid id_;
};

class game_scene
{
public:
    void update(float delta_time) const
    {
        //physics_world_.Step(delta_time, 8, 3);
        for (const auto& game_object : game_objects_)
        {
            game_object->update(delta_time);
        }
    }

    void add_game_object(const std::shared_ptr<game_object>& game_object)
    {
        game_objects_.push_back(game_object);
    }

    void remove_game_object(const std::shared_ptr<game_object>& game_object)
    {
        for (auto it = game_objects_.begin(); it != game_objects_.end(); ++it)
        {
            if (*it == game_object)
            {
                game_objects_.erase(it);
                break;
            }
        }
    }

private:
    std::vector<std::shared_ptr<game_object>> game_objects_{};

    //b2World physics_world_{ { 0.0f, 0.0f } };
};

class render_object : public sf::Transformable
{
    friend container;

public:
    virtual ~render_object() override = default;

public:
    void free()
    {
        if (parent_ != nullptr)
        {
            //TODO: fix
            //parent_->remove_child(std::shared_ptr<render_object>(this));
            //parent_ = nullptr;
        }
    }

protected:
    virtual void draw(const std::shared_ptr<sf::RenderTarget>& render_target, const sf::Transform& parent_transform) const = 0;

private:
    std::shared_ptr<container> parent_{ nullptr };
};

class container : public render_object
{
public:
    virtual ~container() override = default;

public:
    void add_child(const std::shared_ptr<render_object>& child)
    {
        if (child->parent_ != nullptr)
        {
            child->parent_->remove_child(child);
        }

        children_.push_back(child);
        //child->parent_ = std::shared_ptr<container>(this); //TODO: fix
    }

    void remove_child(const std::shared_ptr<render_object>& child)
    {
        for (auto it = children_.begin(); it != children_.end(); ++it)
        {
            if (*it == child)
            {
                children_.erase(it);
                break;
            }
        }
    }

protected:
    void draw(const std::shared_ptr<sf::RenderTarget>& render_target, const sf::Transform& parent_transform) const override
    {
        const sf::Transform transform = parent_transform * getTransform();
        for (const auto& ch : children_)
        {
            ch->draw(render_target, transform);
        }
    }

protected:
    std::vector<std::shared_ptr<render_object>> children_{};
};

class render_scene final : public container
{
public:
    explicit render_scene(const std::shared_ptr<sf::RenderTarget>& render_target) : render_target_{ render_target }
    {
    }

    virtual ~render_scene() override = default;

public:
    void draw_scene() const
    {
        container::draw(render_target_, getTransform());
    }

private:
    std::shared_ptr<sf::RenderTarget> render_target_{ nullptr };
};

class texture_render_data
{
public:
    explicit texture_render_data(const std::string& path)
    {
        texture_ = std::make_shared<sf::Texture>();
        texture_->loadFromFile(path);
        texture_->setSmooth(true);
        texture_size_ = texture_->getSize();
    }

public:
    std::shared_ptr<sf::Texture> get_texture() const
    {
        return texture_;
    }

    sf::Vector2u get_size() const
    {
        return texture_size_;
    }

private:
    std::shared_ptr<sf::Texture> texture_{ nullptr };
    sf::Vector2u texture_size_{};
};


class sprite : public render_object
{
public:
    explicit sprite(const std::string& path)
    {
        texture_render_data_ = std::make_shared<texture_render_data>(path);
        sprite_.setTexture(*texture_render_data_->get_texture());
    }

    explicit sprite(const std::shared_ptr<texture_render_data>& texture_render_data)
    {
        texture_render_data_ = texture_render_data;
        sprite_.setTexture(*texture_render_data_->get_texture());
    }

    virtual ~sprite() override = default;

protected:
    void draw(const std::shared_ptr<sf::RenderTarget>& render_target, const sf::Transform& parent_transform) const override
    {
        if (texture_render_data_ == nullptr)
        {
            return;
        }
        render_target->draw(sprite_, parent_transform * getTransform());
    }

private:
    sf::Sprite sprite_{};
    std::shared_ptr<texture_render_data> texture_render_data_{ nullptr };
};

//TODO:: create object poll for such objects
class bullet : public game_object
{
public:
    bullet(const uuid id, float velocity, const std::shared_ptr<texture_render_data>& render_data, b2Body* body) :
        game_object(id), velocity_{ velocity }, render_data_{ render_data }
    {
        physics_body_ = body ;
    }

    virtual ~bullet() override = default;

public:
    virtual void initialize() override
    {
        render_object_ = std::make_shared<sprite>(render_data_);
    }

    virtual void update(float delta_time) override
    {
        if (render_object_ == nullptr || velocity_ == 0.0f)
        {
            return;
        }

        /*
        const auto position = render_object_->getPosition();
        const auto rotation = render_object_->getRotation();
        const auto rad_rotation = static_cast<float>(std::numbers::pi * static_cast<double>(rotation) / 180.0);

        const auto new_position_x = position.x + std::sin(rad_rotation) * velocity_ * delta_time;
        const auto new_position_y = position.y + -std::cos(rad_rotation) * velocity_ * delta_time;

        render_object_->setPosition(new_position_x, new_position_y);
        */
        auto& ph_body_pos = physics_body_->GetPosition();
        const float ph_body_rot = physics_body_->GetAngle();

        render_object_->setPosition({ ph_body_pos.x * 10.0f, ph_body_pos.y * 10.0f });
        render_object_->setRotation(std::fmod(
            static_cast<float>(static_cast<double>(ph_body_rot) * 180.0f / std::numbers::pi), 360.0f)
        );

        //TODO: delete bullet if it out of game field (field_size_)
    }

    virtual void free() override
    {
        if (render_object_ != nullptr)
        {
            render_object_->free();
            render_object_ = nullptr;
        }
    }

private:
    float velocity_{ 0.0f };
    sf::Vector2f position_{};
    sf::Vector2u field_size_{};

    std::shared_ptr<texture_render_data> render_data_{ nullptr };
};

class physics_contact_listener final : public b2ContactListener
{
    void BeginContact(b2Contact* contact) override
    {
        b2Fixture* fixture_a = contact->GetFixtureA();
        b2Fixture* fixture_b = contact->GetFixtureB();

        // Check if fixtureA or fixtureB is your object, and perform actions accordingly.
        // For example, you can check user data associated with the fixtures.
        b2Body* body_a = fixture_a->GetBody();
        b2Body* body_b = fixture_b->GetBody();
        b2BodyUserData user_data_a = body_a->GetUserData();
        b2BodyUserData user_data_b = body_b->GetUserData();

        const game_object_b2d_link* link_a{ nullptr };
        const game_object_b2d_link* link_b{ nullptr };

        if (user_data_a.pointer != 0)
        {
            link_a = reinterpret_cast<game_object_b2d_link*>(user_data_a.pointer);
            if (link_a->type == game_object_type::bullet)
            {
                //std::cout << "Bullet collision detected!" << std::endl;
            }
        }

        if (user_data_b.pointer != 0)
        {
            link_b = reinterpret_cast<game_object_b2d_link*>(user_data_b.pointer);
            if (link_b->type == game_object_type::bullet)
            {
                //std::cout << "Bullet collision detected!" << std::endl;
            }
        }

        // Perform collision handling here.
        //std::cout << "Collision detected!" << std::endl;
    }
};

int main()
{
    constexpr float pixels_per_meters{ 10.0f }; // 1 meter = 10 pixels
    const sf::Vector2u game_field_size{ 800, 600 };

    auto window = std::make_shared<sf::RenderWindow>(sf::VideoMode(game_field_size.x, game_field_size.y), "Battle Tanks");
    //window->setFramerateLimit(20);

    sf::Clock clock;
    game_scene game;
    render_scene root_node{window};
    b2World physics_world{ { 0.0f, 0.0f } };
    physics_world.SetAllowSleeping(true);
    //physics_world.SetContinuousPhysics(true);
    auto* contact_listener = new physics_contact_listener();
    physics_world.SetContactListener(contact_listener);
    SFMLDebugDraw draw{ *window, pixels_per_meters };
    draw.AppendFlags(b2Draw::e_shapeBit);
    physics_world.SetDebugDraw(&draw);

    // <TEXTURES>
    auto tank_texture_render_data = std::make_shared<texture_render_data>("game_data/atlases/tank.png");
    auto tower_texture_render_data = std::make_shared<texture_render_data>("game_data/atlases/tower.png");
    auto bullet_texture_render_data = std::make_shared<texture_render_data>("game_data/atlases/bullet_1.png");
    // </TEXTURES>

    // <TANK>
    move_action tank_move_action{};
    rotate_action tank_rotate_action{};
    rotate_action tower_rotate_action{};
    bool tank_fire{ false };

    sf::Vector2u tank_texture_size = tank_texture_render_data->get_size();

    const auto tank_container = std::make_shared<container>();
    tank_container->setPosition(100.0, 100.0);
    tank_container->setOrigin(static_cast<float>(tank_texture_size.x) / 2.0f, static_cast<float>(tank_texture_size.y) / 2.0f);
    root_node.add_child(tank_container);

    const auto tank_base_sprite = std::make_shared<sprite>(tank_texture_render_data);
    tank_container->add_child(tank_base_sprite);

    const auto tank_tower_sprite = std::make_shared<sprite>(tower_texture_render_data);
    tank_tower_sprite->setPosition(16.5f, 27.0f);
    tank_tower_sprite->setOrigin(10.0f, 67.0f);
    tank_container->add_child(tank_tower_sprite);

    b2BodyDef tank_body_def;
    tank_body_def.type = b2_dynamicBody;
    tank_body_def.position.Set(100.0f / pixels_per_meters, 100.0f / pixels_per_meters);
    auto tank_body = physics_world.CreateBody(&tank_body_def);
    b2PolygonShape tank_shape;
    tank_shape.SetAsBox(
        static_cast<float>(tank_texture_size.x) / pixels_per_meters / 2,
        static_cast<float>(tank_texture_size.y) / pixels_per_meters / 2
    );
    tank_body->CreateFixture(&tank_shape, 1.0f);
    // </TANK>

    // <EDGES>
    b2Vec2 bottom_right_point = b2Vec2{
            static_cast<float>(game_field_size.x) / pixels_per_meters,
            static_cast<float>(game_field_size.y) / pixels_per_meters
    };
    b2BodyDef edge_body_def;
    edge_body_def.type = b2_staticBody;
    auto edges_body = physics_world.CreateBody(&edge_body_def);

    b2EdgeShape top_edge_shape;
    top_edge_shape.SetTwoSided(b2Vec2(0.0f, 0.0f), b2Vec2(bottom_right_point.x, 0.0f));
    edges_body->CreateFixture(&top_edge_shape, 1.0f);

    b2EdgeShape bottom_edge_shape;
    bottom_edge_shape.SetTwoSided(b2Vec2(0.0f, bottom_right_point.y), bottom_right_point);
    edges_body->CreateFixture(&bottom_edge_shape, 1.0f);

    b2EdgeShape left_edge_shape;
    left_edge_shape.SetTwoSided(b2Vec2(0.0f, 0.0f), b2Vec2(0.0f, bottom_right_point.y));
    edges_body->CreateFixture(&left_edge_shape, 1.0f);

    b2EdgeShape right_edge_shape;
    right_edge_shape.SetTwoSided(b2Vec2(bottom_right_point.x, 0.0f), bottom_right_point);
    edges_body->CreateFixture(&right_edge_shape, 1.0f);
    // </EDGES>

    auto bullet_creation_time = std::chrono::high_resolution_clock::now();
    while (window->isOpen())
    {
        float delta_time = clock.restart().asSeconds();

        sf::Event event;
        while (window->pollEvent(event))
        {
            if (event.type == sf::Event::Closed ||
                (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Key::Escape))
            {
                window->close();
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
            {
                tank_move_action = move_action::forward;
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
            {
                tank_move_action = move_action::backward;
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
            {
                tank_rotate_action = left;
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
            {
                tank_rotate_action = right;
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
            {
                tower_rotate_action = left;
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
            {
                tower_rotate_action = right;
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
            {
                tank_fire = true;
            }

            if (event.type == sf::Event::KeyReleased)
            {
                if (event.key.code == sf::Keyboard::W || event.key.code == sf::Keyboard::S)
                {
                    tank_move_action = move_action::stop;
                }

                if (event.key.code == sf::Keyboard::A || event.key.code == sf::Keyboard::D)
                {
                    tank_rotate_action = stop;
                }

                if (event.key.code == sf::Keyboard::Left || event.key.code == sf::Keyboard::Right)
                {
                    tower_rotate_action = stop;
                }

                if (event.key.code == sf::Keyboard::Space)
                {
                    tank_fire = false;
                }
            }
        }

        if (tank_fire)
        {
            auto bullet_current_time = std::chrono::high_resolution_clock::now();

            if (std::chrono::duration_cast<std::chrono::milliseconds>(bullet_current_time - bullet_creation_time).count() >= 1000)
            {
                bullet_creation_time = bullet_current_time;
                const float bullet_rotation_rad = deg_to_rad(tank_tower_sprite->getRotation() + tank_container->getRotation());
                const b2Vec2 bullet_pos{
                (tank_container->getPosition().x + 80 * std::sin(bullet_rotation_rad)) / pixels_per_meters,
                (tank_container->getPosition().y - 80 * std::cos(bullet_rotation_rad)) / pixels_per_meters
                };
                b2BodyDef bullet_body_def;
                bullet_body_def.type = b2_dynamicBody;
                bullet_body_def.position.Set(bullet_pos.x, bullet_pos.y);
                auto bullet_body = physics_world.CreateBody(&bullet_body_def);
                b2PolygonShape bullet_shape;
                bullet_shape.SetAsBox(
                    static_cast<float>(bullet_texture_render_data->get_size().x) / pixels_per_meters / 2,
                    static_cast<float>(bullet_texture_render_data->get_size().y) / pixels_per_meters / 2
                );
                bullet_body->CreateFixture(&bullet_shape, 1.0f);
                uuid bullet_id = generate_uuid();
                auto* bullet_link = new game_object_b2d_link{ game_object_type::bullet, bullet_id };
                bullet_body->GetUserData().pointer = reinterpret_cast<uintptr_t>(bullet_link);

                auto force = b2Vec2{
                    std::sin(bullet_rotation_rad) * 500.0f / pixels_per_meters,
                    -std::cos(bullet_rotation_rad) * 500.0f / pixels_per_meters
                };
                bullet_body->SetTransform(bullet_pos, bullet_rotation_rad);
                bullet_body->SetLinearVelocity(force);

                const auto bt = std::make_shared<bullet>(bullet_id, 500.0f, bullet_texture_render_data, bullet_body);
                bt->initialize();
                bt->get_render_object()->setOrigin(
                    static_cast<float>(bullet_texture_render_data->get_size().x) / 2.0f,
                    static_cast<float>(bullet_texture_render_data->get_size().y) / 2.0f
                );
                game.add_game_object(bt);
                root_node.add_child(bt->get_render_object());
            }
        }

        const auto rad_rotation = tank_body->GetAngle();
        auto tower_rotation = tank_tower_sprite->getRotation();

        constexpr float move_speed = 100.0;
        if (tank_move_action == move_action::forward)
        {
            auto force = b2Vec2{
                    std::sin(rad_rotation) * move_speed / pixels_per_meters,
                    -std::cos(rad_rotation) * move_speed / pixels_per_meters
            };

            tank_body->SetLinearVelocity(force);
        }
        else if (tank_move_action == move_action::backward)
        {
            auto force = b2Vec2{
                    -std::sin(rad_rotation) * move_speed / pixels_per_meters,
                    std::cos(rad_rotation) * move_speed / pixels_per_meters
            };

            tank_body->SetLinearVelocity(force);
        }
        else
        {
            tank_body->SetLinearVelocity({ 0.0f, 0.0f });
        }

        constexpr float rotate_speed = 90.0f;
        constexpr float rotate_speed_rad = static_cast<float>(std::numbers::pi) / 2.0f;
        if (tank_rotate_action == left)
        {
            tank_body->SetAngularVelocity(-rotate_speed_rad);
        }
        else if (tank_rotate_action == right)
        {
            tank_body->SetAngularVelocity(rotate_speed_rad);
        }
        else
        {
            tank_body->SetAngularVelocity(0.0f);
        }

        if (tower_rotate_action == left)
        {
            tower_rotation -= rotate_speed * delta_time;
        }
        else if (tower_rotate_action == right)
        {
            tower_rotation += rotate_speed * delta_time;
        }

        auto& ph_body_pos = tank_body->GetPosition();
        float ph_body_rot = tank_body->GetAngle();

        tank_container->setPosition({ph_body_pos.x * pixels_per_meters, ph_body_pos.y * pixels_per_meters});
        tank_container->setRotation(std::fmod(
            static_cast<float>(static_cast<double>(ph_body_rot) * 180.0f / std::numbers::pi), 360.0f)
        );
        tank_tower_sprite->setRotation(tower_rotation);

        window->clear(sf::Color::Black);

        physics_world.Step(delta_time, 8, 3);
        game.update(delta_time);
        root_node.draw_scene();
        physics_world.DebugDraw();

        window->display();
    }

    physics_world.DestroyBody(tank_body);
    tank_body = nullptr;

    return 0;
}
