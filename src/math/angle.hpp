#pragma once

#include <array>
#include <cstdint>
#include <glm/glm.hpp>

#define ANGLE_CONST 11930464.71
#define DEG_2_ANGLE(deg) (uint32_t)(deg * ANGLE_CONST)
#define ANGLE_2_DEG(angle) (angle / ANGLE_CONST)
#define ANGLE_VAL_0_DEG 0x00000000
#define ANGLE_VAL_45_DEG 0x20000000
#define ANGLE_VAL_90_DEG 0x40000000
#define ANGLE_VAL_135_DEG 0x60000000
#define ANGLE_VAL_180_DEG 0x80000000
#define ANGLE_VAL_225_DEG 0xA0000000
#define ANGLE_VAL_270_DEG 0xC0000000
#define ANGLE_VAL_315_DEG 0xE0000000
#define ANGLE_VAL_360_DEG 0x100000000
#define ANGLE_VAL_PI_RAD ANGLE_VAL_180_DEG
#define ANGLE_VAL_2PI_RAD ANGLE_VAL_360_DEG
#define ANGLE_SIN_COS_SIZE 0x2000

template <std::size_t N, typename F = std::identity>
inline constexpr auto GEN_LOOKUP_TABLE(const F& f = F{})
{
    std::array<float, N> arr;
    for (std::size_t i = 0; i < N; i++)
        arr[i] = f(static_cast<float>(i));
    return arr;
}

// Sine/cosine table. Converts to radians.
inline static auto SIN_COS_TABLE = GEN_LOOKUP_TABLE<ANGLE_SIN_COS_SIZE>([](auto f)
{
    return glm::cos(glm::radians(f * (ANGLE_VAL_360_DEG / ANGLE_SIN_COS_SIZE) / ANGLE_CONST));
});

// For dealing with angular math efficiently.
struct MAngle
{
    uint32_t val; // Actual value.

    // Create an angle from degrees.
    inline constexpr explicit MAngle(long double deg) : val(DEG_2_ANGLE(deg)) {}
    inline constexpr explicit MAngle(float deg) : val(DEG_2_ANGLE(deg)) {}

    // Empty angle.
    inline constexpr MAngle() : val(0) {}

    // Create an angle from a binary angle value.
    inline constexpr MAngle(uint32_t val) : val(val) {}

    // Get the cosine of a value.
    inline float Cos() const
    {
        return SIN_COS_TABLE[val / (uint32_t)(ANGLE_VAL_360_DEG / ANGLE_SIN_COS_SIZE)];
    }

    // Get the sine of a value.
    inline float Sin() const
    {
        return SIN_COS_TABLE[(ANGLE_VAL_90_DEG - val) / (uint32_t)(ANGLE_VAL_360_DEG / ANGLE_SIN_COS_SIZE)];
    }

    // Linear interpolation with another angle.
    inline static MAngle Lerp(MAngle a, MAngle b, float alpha)
    {
        return MAngle(a.val + (uint32_t)((b.val - a.val) * alpha));
    }

    // Operators.
    inline constexpr MAngle operator -() const { return MAngle((uint32_t)(ANGLE_VAL_360_DEG - val)); }
    inline void operator +=(MAngle ang) { val += ang.val; }
    inline constexpr MAngle operator +(MAngle ang) const { return MAngle(val + ang.val); }
    inline void operator -=(MAngle ang) { val -= ang.val; }
    inline constexpr MAngle operator -(MAngle ang) const { return MAngle(val - ang.val); }
    inline void operator /=(unsigned int divisor) { val /= divisor; }
    inline constexpr MAngle operator /(unsigned int divisor) const { return MAngle(val / divisor); }
    inline void operator *=(float multiplier) { val *= multiplier; }
    inline constexpr MAngle operator *(float multiplier) const { return MAngle((uint32_t)(val * multiplier)); }
    inline constexpr bool operator ==(MAngle ang) const { return val == ang.val; }
	inline constexpr bool operator !=(MAngle ang) const { return val != ang.val; }
	inline constexpr bool operator <(MAngle ang) const { return val < ang.val; }
	inline constexpr bool operator <=(MAngle ang) const { return val <= ang.val; }
	inline constexpr bool operator >(MAngle ang) const { return val > ang.val; }
	inline constexpr bool operator >=(MAngle ang) const { return val >= ang.val; }
    inline constexpr explicit operator float() const { return ANGLE_2_DEG(val); }

};

// Make a binary angle from degrees.
constexpr MAngle operator ""_deg(long double val) { return MAngle(val); }

// Make a binary angle from radians.
constexpr MAngle operator ""_rad(long double val) { return MAngle(glm::degrees(val)); }