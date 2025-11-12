#include "renderer/render_object.hpp"
#include "renderer/container.hpp"


void bt::render_object::free()
{
    if (parent_ != nullptr)
    {
        parent_->remove_child(this);
        parent_ = nullptr;
    }
}
