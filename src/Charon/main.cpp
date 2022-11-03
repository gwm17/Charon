#include "Application.h"
#include <iostream>

int main(int argc, char** argv)
{
    std::cout << "Charon guides wayward souls to their place of rest" << std::endl;

    Charon::Application ferryman(32304,"ring_name");
    ferryman.StartServer();
    ferryman.Run();

}
