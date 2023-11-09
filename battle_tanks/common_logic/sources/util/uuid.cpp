#include "utils/uuid.hpp"

namespace
{
    bt::uuid current_uuid{ 1 };
}


bt::uuid bt::generate_uuid()
{
    return current_uuid++;
}
