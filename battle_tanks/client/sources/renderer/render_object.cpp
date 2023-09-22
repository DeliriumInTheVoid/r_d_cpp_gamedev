#include "renderer/render_object.hpp"
#include "renderer/container.hpp"


void render_object::free()
{
    if (parent_ != nullptr)
    {
        parent_->remove_child(this);
        parent_ = nullptr;
    }
}
