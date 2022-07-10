#include <dory.h>


int main()
{
    int var1 = 1;
    char var2[] = "string";
    DORY::DTRACE("This is a trace message.", var1, var2);
    DORY::DDEBUG("This is a debug message.", var1, var2);
    DORY::DINFO("This is a info message.", var1, var2);
    DORY::DWARN("This is a warn message.", var1, var2);
    DORY::DERROR("This is a error message.", var1, var2);
    DORY::DFATAL("This is a fatal message.", var1, var2);

    return 0;
}