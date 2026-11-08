#pragma once

namespace axiom
{
    /// Logic checks run by `Editor --smoke N`. Each failure logs an [ERROR] line, which fails the smoke run.
    void RunGizmoMathChecks();
}
