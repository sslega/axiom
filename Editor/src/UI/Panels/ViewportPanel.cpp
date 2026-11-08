#include "ViewportPanel.h"
#include "Renderer/RenderSubsystem.h"
#include "Renderer/OutlinePostProcess.h"
#include "Scene/Entity.h"
#include "Scene/TransformComponent.h"
#include "Reflection/Reflection.h"
#include "UI/Selection.h"
#include "UI/GizmoMath.h"
#include "UI/Commands/UndoStack.h"
#include "UI/Commands/PropertyEditCommand.h"
#include "Viewport.h"

#include <imgui.h>
#include <ImGuizmo.h>

#include <algorithm>
#include <cmath>

namespace axiom
{
    namespace
    {
        constexpr float kTranslateSnap = 1.0f;
        constexpr float kRotateSnapDegrees = 15.0f;
        constexpr float kScaleSnap = 0.1f;
        constexpr float kGizmoSizeClipSpace = 0.2f; // ImGuizmo's default of 0.1 is too small to grab comfortably

        // The size of the axis a scale drag is working on. The centre handle moves every axis, so it
        // uses the largest, which gives the finest steps.
        float ScaleSnapReference(Vec3 start, ImGuizmo::MOVETYPE handle)
        {
            switch (handle)
            {
                case ImGuizmo::MT_SCALE_X: return std::abs(start.x);
                case ImGuizmo::MT_SCALE_Y: return std::abs(start.y);
                case ImGuizmo::MT_SCALE_Z: return std::abs(start.z);
                default: return std::max({ std::abs(start.x), std::abs(start.y), std::abs(start.z) });
            }
        }

        ImGuizmo::OPERATION ToOperation(GizmoMode mode)
        {
            switch (mode)
            {
                case GizmoMode::Rotate: return ImGuizmo::ROTATE;
                case GizmoMode::Scale:  return ImGuizmo::SCALE;
                default:                return ImGuizmo::TRANSLATE;
            }
        }

        const char* FieldNameFor(GizmoMode mode)
        {
            switch (mode)
            {
                case GizmoMode::Rotate: return "rotation";
                case GizmoMode::Scale:  return "scale";
                default:                return "position";
            }
        }

        // ImGuizmo ends a scale or rotate drag only while the cursor is over its window, so a release
        // anywhere else would leave it stuck in the dragging state.
        void ReleaseImGuizmoDrag(const void* id)
        {
            ImGuizmo::PushID(id);
            if (ImGuizmo::IsUsing())
            {
                ImGuizmo::Enable(false);
                ImGuizmo::Enable(true);
            }
            ImGuizmo::PopID();
        }
    }

    ViewportPanel::ViewportPanel(const String& name, RenderSubsystem& renderSubsystem, Selection& selection, UndoStack& undoStack, GizmoState& gizmoState)
    : ViewportPanel(name, 320, 240, renderSubsystem, selection, undoStack, gizmoState)
    {
    }

    ViewportPanel::ViewportPanel(const String& name, uint32 width, uint32 height, RenderSubsystem& renderSubsystem, Selection& selection, UndoStack& undoStack, GizmoState& gizmoState)
    : EditorPanel(name)
    , m_renderSubsystem(renderSubsystem)
    , m_selection(selection)
    , m_undoStack(undoStack)
    , m_gizmoState(gizmoState)
    {
        axiom::FramebufferSpec frameBufferSpec;
        frameBufferSpec.width = width;
        frameBufferSpec.height = height;
        frameBufferSpec.frameBufferFormat = FramebufferFormat::RGB8;

        SharedPtr<FrameBuffer> frameBuffer = renderSubsystem.GetGraphicsDevice().CreateFrameBuffer(frameBufferSpec);

        m_viewport = MakeUnique<Viewport>(frameBuffer, m_name);
    }

    ViewportPanel::~ViewportPanel() = default;

    void ViewportPanel::OnRender()
    {
        m_viewport->OnRender();
        m_renderSubsystem.SubmitView(m_viewport->GetView());

        // ImGui starts moving a floating window at the end of the click frame, and ImGuizmo never claims the
        // click as an item would. Last frame's hover is the only thing known early enough to prevent it.
        const ImGuiWindowFlags windowFlags = m_gizmoWantsMouse ? ImGuiWindowFlags_NoMove : ImGuiWindowFlags_None;
        const bool visible = ImGui::Begin(m_viewport->GetName().c_str(), nullptr, windowFlags);
        UpdateCameraActive();
        ImVec2 available = ImGui::GetContentRegionAvail();

        uint32 desiredW = (uint32)available.x;
        uint32 desiredH = (uint32)available.y;

        if (desiredW > 0 &&
            desiredH > 0 &&
            (desiredW != m_viewport->GetWidth() || desiredH != m_viewport->GetHeight()))
        {
            m_viewport->Resize(desiredW, desiredH);
        }

        uint32 colorId = m_viewport->GetFrameBuffer()->GetColorAttachmentID();
        ImGui::Image((ImTextureID)(uintptr_t)colorId,
            ImVec2((float)desiredW, (float)desiredH),
            ImVec2(0,1), ImVec2(1,0));

        if (visible)
        {
            // The image's rect, read after any resize above, keeps the gizmo aligned with what is on screen.
            const ImVec2 imagePos = ImGui::GetItemRectMin();
            const ImVec2 imageSize = ImGui::GetItemRectSize();

            HandleModeKeys();
            DrawGizmo(imagePos, imageSize);
            DrawToolbar(imagePos);
        }
        else
        {
            m_gizmoWantsMouse = false;
            if (m_drag)
            {
                EndDrag();
            }
        }

        ImGui::End();
    }

    void ViewportPanel::OnUpdate(float deltaTime)
    {
        m_viewport->OnUpdate(deltaTime);
    }

    void ViewportPanel::UpdateCameraActive()
    {
        const bool rightMouseDown = ImGui::IsMouseDown(ImGuiMouseButton_Right);

        // Hover is lost while the cursor is locked, so remember that this viewport started the look.
        // The camera only reads this on the next update, which is what catches the press itself.
        m_looking = rightMouseDown && (m_looking || m_cameraActive);

        const bool hoveredIdle = !rightMouseDown && ImGui::IsWindowHovered();
        m_cameraActive = ImGui::IsWindowFocused() && !m_gizmoState.dragging && (m_looking || hoveredIdle);
        m_viewport->SetActive(m_cameraActive);
    }

    void ViewportPanel::HandleModeKeys()
    {
        // A mode change mid-drag would swap the field being edited.
        if (!ImGui::IsWindowFocused() || m_gizmoState.dragging)
        {
            return;
        }

        const ImGuiIO& io = ImGui::GetIO();
        if (io.WantTextInput || io.KeyCtrl || ImGui::IsMouseDown(ImGuiMouseButton_Right))
        {
            return;
        }

        if (ImGui::IsKeyPressed(ImGuiKey_W, false))
        {
            m_gizmoState.mode = GizmoMode::Translate;
        }
        else if (ImGui::IsKeyPressed(ImGuiKey_E, false))
        {
            m_gizmoState.mode = GizmoMode::Rotate;
        }
        else if (ImGui::IsKeyPressed(ImGuiKey_R, false))
        {
            m_gizmoState.mode = GizmoMode::Scale;
        }
        else if (ImGui::IsKeyPressed(ImGuiKey_Q, false) || ImGui::IsKeyPressed(ImGuiKey_Escape, false))
        {
            m_gizmoState.mode = GizmoMode::None;
        }
    }

    void ViewportPanel::DrawGizmo(const ImVec2& imagePos, const ImVec2& imageSize)
    {
        m_gizmoWantsMouse = false;

        Entity* entity = m_selection.Get();
        TransformComponent* transform = entity ? entity->GetComponent<TransformComponent>() : nullptr;
        const GizmoMode mode = m_gizmoState.mode;

        // The selection can't normally change during a drag. If it does, the old component may be gone,
        // so end the drag without touching it.
        if (m_drag && m_drag->transform != transform)
        {
            ReleaseImGuizmoDrag(this);
            m_drag.reset();
            m_gizmoState.dragging = false;
        }

        const bool canShow = transform
            && mode != GizmoMode::None
            && imageSize.x > 0.0f && imageSize.y > 0.0f
            && IsFinite(transform->position) && IsFinite(transform->rotation) && IsFinite(transform->scale);
        if (!canShow)
        {
            if (m_drag)
            {
                EndDrag();
            }
            return;
        }

        const Camera& camera = m_viewport->GetCamera().GetCamera();
        Matrix4 gizmoMatrix = ComposeGizmoMatrix(*transform);

        const bool snapping = ImGui::GetIO().KeyCtrl;

        ImGuizmo::PushID(this);
        ImGuizmo::SetOrthographic(false);
        ImGuizmo::SetDrawlist();
        ImGuizmo::SetRect(imagePos.x, imagePos.y, imageSize.x, imageSize.y);
        ImGuizmo::SetGizmoSizeClipSpace(kGizmoSizeClipSpace);

        // Translate and scale snap to absolute multiples in ApplyDrag; ImGuizmo would only snap the drag's offset.
        // Scale still gets a step here, because ImGuizmo draws its handles from its own multiplier.
        float snapStep = 0.0f;
        if (snapping && mode == GizmoMode::Rotate)
        {
            snapStep = kRotateSnapDegrees;
        }
        else if (snapping && mode == GizmoMode::Scale && m_drag)
        {
            snapStep = ScaleMultiplierSnap(ScaleSnapReference(m_drag->startValue, ImGuizmo::GetActiveHandleType()), kScaleSnap);
        }
        const float* snap = snapStep > 0.0f ? &snapStep : nullptr;

        // All gizmos share one ImGuizmo state, so a drag in another viewport is ignored explicitly here.
        const bool otherViewportDragging = m_gizmoState.dragging && !m_drag;
        if (otherViewportDragging)
        {
            ImGuizmo::Enable(false);
        }

        const bool changed = ImGuizmo::Manipulate(
            camera.GetViewMatrix().data, camera.GetProjectionMatrix().data,
            ToOperation(mode), ImGuizmo::WORLD,
            gizmoMatrix.data, nullptr, snap);

        if (otherViewportDragging)
        {
            ImGuizmo::Enable(true);
        }

        const bool usingGizmo = ImGuizmo::IsUsing();
        const bool hovered = ImGuizmo::GetHoveredHandleType() != ImGuizmo::MT_NONE;
        ImGuizmo::PopID();

        if (usingGizmo && !m_drag)
        {
            BeginDrag(*transform, mode);
        }
        if (changed && m_drag)
        {
            ApplyDrag(gizmoMatrix, snapping);
        }
        if (m_drag && (!usingGizmo || !ImGui::IsMouseDown(ImGuiMouseButton_Left)))
        {
            EndDrag();
        }

        m_gizmoWantsMouse = hovered || m_drag.has_value();
    }

    void ViewportPanel::DrawToolbar(const ImVec2& imagePos)
    {
        struct Button { const char* label; GizmoMode mode; };
        static constexpr Button buttons[] = {
            { "Select", GizmoMode::None },
            { "Translate", GizmoMode::Translate },
            { "Rotate", GizmoMode::Rotate },
            { "Scale", GizmoMode::Scale },
        };

        ImGui::SetCursorScreenPos(ImVec2(imagePos.x + 6.0f, imagePos.y + 6.0f));
        bool first = true;
        for (const Button& button : buttons)
        {
            if (!first)
            {
                ImGui::SameLine(0.0f, 2.0f);
            }
            first = false;

            const bool active = m_gizmoState.mode == button.mode;
            if (active)
            {
                ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_ButtonActive));
            }
            if (ImGui::SmallButton(button.label) && !m_gizmoState.dragging)
            {
                m_gizmoState.mode = button.mode;
            }
            if (active)
            {
                ImGui::PopStyleColor();
            }
        }
    }

    void ViewportPanel::BeginDrag(TransformComponent& transform, GizmoMode mode)
    {
        const TypeDescriptor* typeDescriptor = Reflection::Get().Find(typeid(TransformComponent));
        AX_ASSERT(typeDescriptor, "TransformComponent is not reflected");
        const FieldDescriptor* field = typeDescriptor->FindField(FieldNameFor(mode));
        AX_ASSERT(field, "TransformComponent has no such field");

        m_drag = GizmoDrag{ &transform, field, mode, std::get<Vec3>(field->get(&transform)) };
        m_gizmoState.dragging = true;
    }

    void ViewportPanel::ApplyDrag(const Matrix4& gizmoMatrix, bool snapping)
    {
        TransformComponent& transform = *m_drag->transform;
        const Vec3 start = m_drag->startValue;

        Vec3 value;
        switch (m_drag->mode)
        {
            case GizmoMode::Translate:
            {
                const Vec3 position(gizmoMatrix(0, 3), gizmoMatrix(1, 3), gizmoMatrix(2, 3));
                value = ResolveDraggedAxes(position, start, snapping ? kTranslateSnap : 0.0f, false);
                break;
            }
            case GizmoMode::Rotate:
            {
                value = RotationFromGizmo(gizmoMatrix, GizmoFeedScale(transform.scale), transform.rotation);
                break;
            }
            case GizmoMode::Scale:
            {
                const Vec3 scale = ScaleFromGizmo(gizmoMatrix, ComposeEulerRotation(transform.rotation), start, GizmoFeedScale(start));
                value = ResolveDraggedAxes(scale, start, snapping ? kScaleSnap : 0.0f, true);
                break;
            }
            default:
                return;
        }

        if (IsFinite(value))
        {
            m_drag->field->set(&transform, FieldValue(value));
        }
    }

    void ViewportPanel::EndDrag()
    {
        ReleaseImGuizmoDrag(this);

        const Vec3 newValue = std::get<Vec3>(m_drag->field->get(m_drag->transform));
        if (newValue != m_drag->startValue)
        {
            m_undoStack.Execute(MakeUnique<PropertyEditCommand>(m_drag->transform, m_drag->field, FieldValue(m_drag->startValue), FieldValue(newValue)));
        }

        m_drag.reset();
        m_gizmoState.dragging = false;
    }
}
