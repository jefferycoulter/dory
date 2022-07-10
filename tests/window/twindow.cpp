#include <dory.h>

int main()
{
    DORY::Window window(800, 600, "Dory Window Test");

    while (!window.ShouldClose())
    {
        glfwPollEvents();
    }
    return 0;
}