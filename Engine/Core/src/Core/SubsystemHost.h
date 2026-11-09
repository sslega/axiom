#pragma once

#include "Core/ApplicationSubsystem.h"
#include "Core/Assert.h"
#include "Core/Log.h"
#include "Core/Types.h"

#include <algorithm>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>

namespace axiom
{
    /// Owns the ApplicationSubsystems and runs their lifecycle and frame hooks.
    ///
    /// Hooks run in registration order; shutdown runs in reverse. Application derives from this.
    ///
    /// @code{.cpp}
    /// RenderSubsystem* render = RegisterSubsystem<RenderSubsystem>(window, api); // extra arguments go to its constructor
    /// FileSubsystem& file = GetSubsystem<FileSubsystem>();
    /// @endcode
    class SubsystemHost
    {
    public:
        SubsystemHost() = default;
        SubsystemHost(const SubsystemHost&) = delete;
        SubsystemHost& operator=(const SubsystemHost&) = delete;

        template <typename T>
        T& GetSubsystem()
        {
            auto it = m_applicationSubsystems.find(TypeID<T>());
            AX_ASSERT(it != m_applicationSubsystems.end(), "Module not registered");
            return *static_cast<T*>(it->second.get());
        }

        template <typename T>
        const T& GetSubsystem() const
        {
            auto it = m_applicationSubsystems.find(TypeID<T>());
            AX_ASSERT(it != m_applicationSubsystems.end(), "Module not registered");
            return *static_cast<const T*>(it->second.get());
        }

        template <typename T>
        bool HasSubsystem() const
        {
            return m_applicationSubsystems.find(TypeID<T>()) != m_applicationSubsystems.end();
        }

    protected:
        ~SubsystemHost() = default;

        template <typename T, typename... Args>
        T* RegisterSubsystem(Args&&... args)
        {
            Log::Info("Registering subsystem: {}", typeid(T).name());
            UniquePtr<T> module = MakeUnique<T>(*this, std::forward<Args>(args)...);
            T* ptr = module.get();
            std::type_index id = TypeID<T>();
            m_applicationSubsystems[id] = std::move(module);
            m_subsystemOrder.push_back(id);
            ptr->Register();
            return ptr;
        }

        template <typename T>
        void UnregisterSubsystem()
        {
            std::type_index id = TypeID<T>();
            auto it = m_applicationSubsystems.find(id);
            if (it != m_applicationSubsystems.end())
                it->second->Unregister();
            m_applicationSubsystems.erase(id);
            m_subsystemOrder.erase(
                std::remove(m_subsystemOrder.begin(), m_subsystemOrder.end(), id),
                m_subsystemOrder.end()
            );
        }

        void InitializeSubsystems();
        void ShutdownSubsystems();
        void UpdateSubsystems(float deltaTime);
        void BeginFrameSubsystems();
        void RenderSubsystems();
        void EndFrameSubsystems();

        /// A derived class whose members must outlive the subsystems calls this from its destructor.
        void DestroySubsystems();

    private:
        TypeMap<UniquePtr<ApplicationSubsystem>> m_applicationSubsystems;
        Vector<std::type_index> m_subsystemOrder; // tracks registration order
    };

    template <typename T>
    T& ApplicationSubsystem::GetSubsystem()
    {
        return m_host.GetSubsystem<T>();
    }
}
