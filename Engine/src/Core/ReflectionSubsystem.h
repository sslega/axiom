#pragma once

#include "ApplicationSubsystem.h"
#include "Reflection.h"

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