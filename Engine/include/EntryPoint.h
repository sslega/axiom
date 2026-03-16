#include "Core/Application.h"
#include "Core/Types.h"


extern axiom::UniquePtr<axiom::Application> CreateApplication();

int main()
{
    axiom::UniquePtr<axiom::Application> application = CreateApplication();

    return application->Run();
}