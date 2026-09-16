#include "Reflection/EngineReflection.h"
#include "Reflection/ReflectionField.h"
#include "Core/Object.h"
#include "Scene/Entity.h"
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
    void EngineReflection::Register(TypeRegistry &registry)
    {
        {
            TypeDescriptor desc;
            desc.name = "Object";
            desc.type = typeid(Object);
            desc.fields.push_back(MakeField("id", &Object::m_id));
            registry.Register(std::move(desc));

        }

        {
            TypeDescriptor desc;
            desc.name = "Entity";
            desc.type = typeid(Entity);
            desc.baseType = typeid(Object);
            desc.fields.push_back(MakeField("name", &Entity::m_name));
            registry.Register(std::move(desc));
        }

        {
            TypeDescriptor desc;
            desc.name = "Component";
            desc.type = typeid(Component);
            desc.baseType = typeid(Object);
            registry.Register(std::move(desc));
        }

        {
            TypeDescriptor desc;
            desc.baseType = typeid(Component);
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
            desc.baseType = typeid(Component);
            desc.name = "CameraComponent";
            desc.type = typeid(CameraComponent);
            desc.factory = []() -> UniquePtr<Component> { return MakeUnique<CameraComponent>(); };
            registry.Register(std::move(desc));
        }

        {
            TypeDescriptor desc;
            desc.baseType = typeid(Component);
            desc.name = "CameraController";
            desc.type = typeid(CameraController);
            desc.factory = []() -> UniquePtr<Component> { return MakeUnique<CameraController>(); };
            desc.fields.push_back(MakeField("moveSpeed", &CameraController::moveSpeed));
            desc.fields.push_back(MakeField("lookSensitivity", &CameraController::lookSensitivity));
            registry.Register(std::move(desc));
        }

        {
            TypeDescriptor desc;
            desc.baseType = typeid(Component);
            desc.name = "DirectionalLightComponent";
            desc.type = typeid(DirectionalLightComponent);
            desc.factory = []() -> UniquePtr<Component> { return MakeUnique<DirectionalLightComponent>(); };
            desc.fields.push_back(MakeField("color", &DirectionalLightComponent::color));
            desc.fields.push_back(MakeField("intensity", &DirectionalLightComponent::intensity));
            registry.Register(std::move(desc));
        }

        {
            TypeDescriptor desc;
            desc.baseType = typeid(Component);
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
