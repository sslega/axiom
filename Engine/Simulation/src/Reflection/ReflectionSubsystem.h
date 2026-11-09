#pragma once

#include "Core/ApplicationSubsystem.h"
#include "Reflection/Reflection.h"

namespace axiom
{
    class ReflectionSubsystem : public ApplicationSubsystem
    {
        public:
            using Registrar = void (*)(TypeRegistry&);

            ReflectionSubsystem(SubsystemHost& host);
            inline const TypeRegistry& GetTypeRegistry() const { return m_typeRegistry; };

            /// Added before initialization. Registrars run in the order they were added.
            void AddRegistrar(Registrar registrar) { m_registrars.push_back(registrar); }

        protected:
            virtual void OnInitialize() override;
            virtual void OnShutdown() override;
        
        private:
            TypeRegistry m_typeRegistry;
            Vector<Registrar> m_registrars;
    };
}