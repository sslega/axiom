#include "Reflection/EngineReflection.h"
#include "Reflection/Reflection.h"
#include "Scene/Component.h"
#include "Scene/TransformComponent.h"
#include "Renderer/MeshComponent.h"
#include "Renderer/CameraComponent.h"
#include "Renderer/LightComponent.h"
#include "Renderer/CameraController.h"
#include <cstddef>   // offsetof

namespace axiom
{
    void RegisterEngineReflection(TypeRegistry& registry)
    {
        {
            TypeDescriptor desc;
            desc.name = "TransformComponent";
            desc.type = typeid(TransformComponent);
            desc.factory = []() -> UniquePtr<Component> { return MakeUnique<TransformComponent>(); };
            desc.fields.push_back({ "position", MapFieldType<Vec3>(), offsetof(TransformComponent, position) });
            desc.fields.push_back({ "rotation", MapFieldType<Vec3>(), offsetof(TransformComponent, rotation) });
            desc.fields.push_back({ "scale",    MapFieldType<Vec3>(), offsetof(TransformComponent, scale) });
            registry.Register(std::move(desc));
        }
    
        {
            TypeDescriptor desc;
            desc.name = "CameraComponent";
            desc.type = typeid(CameraComponent);
            desc.factory = []() -> UniquePtr<Component> { return MakeUnique<CameraComponent>(); };
            registry.Register(std::move(desc));
        }

        {
            TypeDescriptor desc;
            desc.name = "CameraController";
            desc.type = typeid(CameraController);
            desc.factory = []() -> UniquePtr<Component> { return MakeUnique<CameraController>(); };
            desc.fields.push_back({ "moveSpeed", MapFieldType<float>(), offsetof(CameraController, moveSpeed) });
            desc.fields.push_back({ "lookSensitivity", MapFieldType<float>(), offsetof(CameraController, lookSensitivity) });
            registry.Register(std::move(desc));
        }

        {
            TypeDescriptor desc;
            desc.name = "DirectionalLightComponent";
            desc.type = typeid(DirectionalLightComponent);
            desc.factory = []() -> UniquePtr<Component> { return MakeUnique<DirectionalLightComponent>(); };
            desc.fields.push_back({ "color", MapFieldType<Vec3>(), offsetof(DirectionalLightComponent, color) });
            desc.fields.push_back({ "intensity", MapFieldType<float>(), offsetof(DirectionalLightComponent, intensity) });
            registry.Register(std::move(desc));
        }

         {
            TypeDescriptor desc;
            desc.name = "MeshComponent";
            desc.type = typeid(MeshComponent);
            desc.factory = []() -> UniquePtr<Component> { return MakeUnique<MeshComponent>(); };
            registry.Register(std::move(desc));
        }
    }
}
