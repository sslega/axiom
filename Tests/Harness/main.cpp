#include "EditorHarness.h"

// After the engine headers: doctest's implementation pulls in windows.h, whose near/far macros break them.
#define DOCTEST_CONFIG_IMPLEMENT
#include <doctest/doctest.h>

#include <crtdbg.h>
#include <cstdlib>
#include <cstring>

namespace
{
    // A failed assert must end an automated run with an error code, not wait on a dialog nobody will click.
    void DisableCrashDialogs()
    {
        _set_error_mode(_OUT_TO_STDERR);
        _set_abort_behavior(0, _WRITE_ABORT_MSG | _CALL_REPORTFAULT);

        for (int reportType : { _CRT_ERROR, _CRT_ASSERT })
        {
            _CrtSetReportMode(reportType, _CRTDBG_MODE_FILE);
            _CrtSetReportFile(reportType, _CRTDBG_FILE_STDERR);
        }
    }

    bool HasFlag(int argc, char** argv, const char* flag)
    {
        for (int i = 1; i < argc; ++i)
        {
            if (std::strcmp(argv[i], flag) == 0)
            {
                return true;
            }
        }
        return false;
    }
}

int main(int argc, char** argv)
{
    DisableCrashDialogs();

    axiom::tests::EditorHarness::SetWindowVisible(HasFlag(argc, argv, "--show-window"));

    doctest::Context context(argc, argv);
    const int result = context.run();

    // Booted only if a runtime case ran, so a logic-only run never opens a window.
    axiom::tests::EditorHarness::ShutdownIfBooted();

    return result;
}
