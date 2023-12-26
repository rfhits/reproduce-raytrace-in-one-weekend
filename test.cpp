#include<iostream>
#include "rtweekend.h"

int main()
{
    vec3 v{ 1,2,3 };
    std::cout << v.x << std::endl;
    std::cout << sizeof(v) << std::endl;
    v.x = 11;
    std::cout << v << std::endl;
    v.x = 3; v.y=4; v.z=0;
    std::cout << v.length() << std::endl;
    vec3 o(v);
    std::cout << o << std::endl;
    std::cout << o+v << std::endl;
    o+=v;
    std::cout << o << std::endl;
    std::cout << o.length() << std::endl;
    std::cout << (int)255.999 << std::endl;
    return 0;
}