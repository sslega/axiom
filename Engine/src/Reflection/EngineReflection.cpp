#include "Reflection/EngineReflection.h"
#include "Reflection/ReflectionField.h"
#include "Scene/Component.h"
#include "Scene/TransformComponent.h"
#include "Renderer/MeshComponent.h"
#include "Renderer/CameraComponent.h"
#include "Renderer/LightComponent.h"
#include "Renderer/CameraController.h"
#include "Resources/MeshResource.h"
#include "Resources/MaterialResource.h"

namespace axiom
{
    void RegisterEngineReflection(TypeRegistry& registry)
    {
        {
            TypeDescriptor desc;
            desc.name = "TransformComponent";
            desc.type = typeid(TransformComponent);
            desc.factory = []() -> UniquePtr<Component> { return MakeUnique<TransformComponent>(); };
            desc.fields.push_back(MakeField("position", &TransformComponent::position));
            desc.fields.push_back(MakeField("rotation", &TransformComponent::rotation));
            desc.fields.push_back(MakeField("scale", &TransformComponent::scale));            
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
            desc.fields.push_back(MakeField("moveSpeed", &CameraController::moveSpeed));
            desc.fields.push_back(MakeField("lookSensitivity", &CameraController::lookSensitivity));
            registry.Register(std::move(desc));
        }

        {
            TypeDescriptor desc;
            desc.name = "DirectionalLightComponent";
            desc.type = typeid(DirectionalLightComponent);
            desc.factory = []() -> UniquePtr<Component> { return MakeUnique<DirectionalLightComponent>(); };
            desc.fields.push_back(MakeField("color", &DirectionalLightComponent::color));
            desc.fields.push_back(MakeField("intensity", &DirectionalLightComponent::intensity));
            registry.Register(std::move(desc));
        }

        {
            TypeDescriptor desc;
            desc.name = "MeshComponent";
            desc.type = typeid(MeshComponent);
            desc.factory = []() -> UniquePtr<Component> { return MakeUnique<MeshComponent>(); };
            desc.fields.push_back(MakeField("visible", &MeshComponent::IsVisible, &MeshComponent::SetVisible));
            desc.fields.push_back(MakeField("mesh",     &MeshComponent::mesh));
            desc.fields.push_back(MakeField("material", &MeshComponent::material));
            registry.Register(std::move(desc));
        }
    }
}
