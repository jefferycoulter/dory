#include <dory.h>

class TestApp : public DORY::Application
{
public:
    TestApp()
    {
        DORY::DTRACE("Hello World!");
    }

    ~TestApp()
    {
        DORY::DTRACE("Goodbye World!");
    }
};

DORY::Application* DORY::Init()
{
    return new TestApp();
}