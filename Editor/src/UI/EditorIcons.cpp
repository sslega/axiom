#include "EditorIcons.h"
#include "Scene/Entity.h"
#include "Scene/TransformComponent.h"
#include "Renderer/CameraComponent.h"
#include "Renderer/LightComponent.h"
#include "Renderer/MeshComponent.h"

#include <IconsFontAwesome6.h>

namespace axiom
{
    const char* EditorIcons::ForComponent(std::type_index type)
    {
        if (type == std::type_index(typeid(TransformComponent))) return ICON_FA_ARROWS_UP_DOWN_LEFT_RIGHT;
        if (type == std::type_index(typeid(MeshComponent))) return ICON_FA_CUBE;
        if (type == std::type_index(typeid(CameraComponent))) return ICON_FA_VIDEO;
        if (type == std::type_index(typeid(DirectionalLightComponent))) return ICON_FA_SUN;
        return ICON_FA_PUZZLE_PIECE;
    }

    const char* EditorIcons::ForEntity(const Entity& entity)
    {
        // An entity's icon says what it is, so the most specific component decides.
        if (entity.HasComponent<CameraComponent>()) return ICON_FA_VIDEO;
        if (entity.HasComponent<DirectionalLightComponent>()) return ICON_FA_SUN;
        if (entity.HasComponent<MeshComponent>()) return ICON_FA_CUBE;
        return ICON_FA_LOCATION_CROSSHAIRS;
    }
}
