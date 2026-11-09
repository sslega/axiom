#pragma once

#include "Math/Matrix4.h"
#include "Math/Vector.h"

namespace axiom
{
    /// Rotation matrix from Euler angles. TransformComponent's convention: `Rx * Ry * Rz`, angles in radians.
    Matrix4 ComposeEulerRotation(Vec3 eulerRadians);

    /// Euler angles of a pure rotation matrix. A rotation has two Euler triples plus turns of 2*PI, so this
    /// returns the one closest to `reference`, which stops the value jumping at the start of a drag.
    Vec3 DecomposeEulerRotation(const Matrix4& rotation, Vec3 reference);
}
