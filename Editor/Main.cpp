#include "Editor.h"
#include "EntryPoint.h"

UniquePtr<Application> CreateApplication()
{
    return MakeUnique<Editor>(Editor::DefaultConfig());
}
