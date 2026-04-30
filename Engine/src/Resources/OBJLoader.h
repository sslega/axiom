#include "Resources/ResourceLoader.h"

namespace axiom
{
    
    class OBJLoader : public ResourceLoader
    {
    public:
        OBJLoader(const ResourceModule& resourceModule);
    
    protected:
        SharedPtr<void> CreateResource(const FileData& fileData) override;
    };
}