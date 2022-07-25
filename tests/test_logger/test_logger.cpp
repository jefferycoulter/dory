#include <dory.h>

class TestApp : public DORY::Application
{
public:
    TestApp()
    {
        int var1 = 1;
        char var2[] = "string";
        DORY::DTRACE("This is a trace message. var1 = %d var2 = %s", var1, var2);
        DORY::DDEBUG("This is a debug message. var1 = %d var2 = %s", var1, var2);
        DORY::DINFO("This is a info message. var1 = %d var2 = %s", var1, var2);
        DORY::DWARN("This is a warn message. var1 = %d var2 = %s", var1, var2);
        DORY::DERROR("This is a error message. var1 = %d var2 = %s", var1, var2);
        DORY::DFATAL("This is a fatal message. var1 = %d var2 = %s", var1, var2);
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