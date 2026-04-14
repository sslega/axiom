#pragma once

namespace axiom
{
    using TimePoint = std::chrono::steady_clock::time_point;
    
    class Timestep
    {
    public:
        Timestep(float seconds = 0.0f)
            : m_seconds(seconds) {}

        float Seconds()      const { return m_seconds; }
        float Milliseconds() const { return m_seconds * 1000.0f; }

        operator float() const { return m_seconds; }

    private:
        float m_seconds;
    };
}
