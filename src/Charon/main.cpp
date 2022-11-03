#include "Application.h"
#include <iostream>

int main(int argc, char** argv)
{
    std::cout << "Charon guides wayward souls to their place of rest" << std::endl;

    Charon::Application ferryman(52324,"tcp://localhost/resolut"); //Idk what the default ring name is but the format is always like this
    ferryman.Run();

}
