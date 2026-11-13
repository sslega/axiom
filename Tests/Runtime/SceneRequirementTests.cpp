#include "RuntimeTest.h"
#include "TestComponents.h"
#include "Core/Log.h"
#include "Scene/TransformComponent.h"
#include "Renderer/LightComponent.h"

namespace axiom::tests
{
    TEST_SUITE("runtime")
    {
        TEST_CASE_FIXTURE(RuntimeTest, "opening a scene adds a missing requirement chain")
        {
            // 1. Baseline. The counter is global and only grows, so measure the difference.
            const uint32 warningsBefore = Log::GetWarningCount();

            // 2. Load inside a frame, the way the UI does it. Only store the result here.
            bool loaded = false;
            DuringNextFrame([&] { loaded = GetUI().LoadScene("tests://Scenes/requirement_chain.scene.json"); });

            // 3. Preconditions. If these fail, the checks below would read garbage, so stop.
            REQUIRE(loaded);
            const Vector<Entity*> entities = GetScene().GetEntities();
            REQUIRE(entities.size() == 1);
            Entity* entity = entities[0];

            // 4. The actual claims. CHECK, so one failure still reports the others.
            CHECK(entity->HasComponent<ReqTop>());
            CHECK(entity->HasComponent<ReqMiddle>());
            CHECK(entity->HasComponent<ReqLeaf>());
            CHECK(Log::GetWarningCount() - warningsBefore == 2);

            // 5. Let a couple of frames run, so anything that breaks after loading shows up here too.
            RunFrames(2);
        }

        TEST_CASE_FIXTURE(RuntimeTest, "opening a light without a transform adds one and keeps the light's values")
        {
            const uint32 warningsBefore = Log::GetWarningCount();

            bool loaded = false;
            DuringNextFrame([&] { loaded = GetUI().LoadScene("tests://Scenes/light_without_transform.scene.json"); });

            REQUIRE(loaded);
            const Vector<Entity*> entities = GetScene().GetEntities();
            REQUIRE(entities.size() == 1);
            Entity* entity = entities[0];

            CHECK(entity->HasComponent<DirectionalLightComponent>());
            CHECK(entity->HasComponent<TransformComponent>());
            const DirectionalLightComponent* light = entity->GetComponent<DirectionalLightComponent>();
            CHECK(light->color == Color{1.5f, 2.95f, 3.21f});
            CHECK(light->intensity == 1.5f);
            CHECK(Log::GetWarningCount() - warningsBefore == 1);

            RunFrames(2);
        }

        TEST_CASE_FIXTURE(RuntimeTest, "the main scene opens with every requirement met")
        {
            const uint32 warningsBefore = Log::GetWarningCount();

            LoadMainScene();

            CHECK(Log::GetWarningCount() - warningsBefore == 0);
            RunFrames(2);
        }
    }
}