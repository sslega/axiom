#include "Resources/ResourceLoader.h"

namespace axiom
{
    
    class OBJLoader : public ResourceLoader
    {
    public:
        OBJLoader(const ResourceSubsystem& resourceModule);
        std::type_index GetResourceType() const override;
    
    protected:
        SharedPtr<Resource> CreateResource(const FileData& fileData) override;
    };
}