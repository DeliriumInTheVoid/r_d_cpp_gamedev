#include <iostream>

#include "vector3d.hpp"


int main()
{
    std::cout << "Home Work 25\n";
    std::cout << "-----------\n\n";

    vector3d v1{ 2, 4, 6 };
    vector3d v2{ 1, 2, 3 };

    std::cout << "vector1 = " << v1 << "\n";
    std::cout << "\tmagnitude = " << v1.magnitude() << "\n";
    std::cout << "\tnegate = " << v1.negate() << "\n";
    v1.scale(3, 3, 3);
    std::cout << "\tscale x3 = " << v1 << "\n";

    std::cout << std::endl;

    std::cout << "vector2 = " << v2 << "\n";
    std::cout << "\tmagnitude = " << v2.magnitude() << "\n";
    std::cout << "\tnegate = " << v2.negate() << "\n";
    v1.scale(2, 2, 2);
    std::cout << "\tscale x3 = " << v1 << "\n";

    std::cout << std::endl;

    std::cout << "dot product = " << v1.dot_product(v2) << "\n";
    std::cout << "cross product = " << v1.cross_product(v2) << "\n";

    std::cout << "relative state = ";
    switch (v1.get_relative_state(v2))
    {
    case vector_relative_state::identical:
        std::cout << "identical";
        break;
    case vector_relative_state::co_directed:
        std::cout << "co_directed";
        break;
    case vector_relative_state::opposite_directed:
        std::cout << "opposite_directed";
        break;
    case vector_relative_state::acute_angle:
        std::cout << "acute_angle";
        break;
    case vector_relative_state::obtuse_angle:
        std::cout << "obtuse_angle";
        break;
    case vector_relative_state::right_angle:
        std::cout << "right_angle";
        break;
    }

    std::cout << std::endl;

    return 0;
}
