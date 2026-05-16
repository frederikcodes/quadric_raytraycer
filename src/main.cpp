#include <iostream>

#include "Ray.h"

int main()
{
    Ray r(Vec3(0, 0, 0), Vec3(1, 0, 0));
    Vec3 p = r.at(5.0);

    std::cout << p.x << " " << p.y << " " << p.z << "\n";
    return 0;
}
