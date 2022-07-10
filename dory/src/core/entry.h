#ifndef DORY_ENTRY_INCL
#define DORY_ENTRY_INCL

#include "core/core.h"

extern DORY::Application* DORY::Init();

int main()
{
    auto app = DORY::Init();
    app->Run();
    delete app;

    return 0;
}

#endif // DORY_ENTRY_INCL