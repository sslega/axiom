#pragma once

#include "Core/ApplicationSubsystem.h"
#include "Reflection/Reflection.h"

namespace axiom
{
    class ReflectionSubsystem : public ApplicationSubsystem
    {
        public:
            ReflectionSubsystem(Application& application);
            inline const TypeRegistry& GetTypeRegistry() const { return m_typeRegistry; };

        protected:
            virtual void OnInitialize() override;
        
        private:
            TypeRegistry m_typeRegistry;
    };
}