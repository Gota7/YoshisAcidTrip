#pragma once

#include <glm/glm.hpp>
#include <yaml-cpp/yaml.h>

namespace YAML
{

    // Mat3.
    template<>
    struct convert<glm::mat3>
    {
        static Node encode(const glm::mat3& rhs);
        static bool decode(const Node& node, glm::mat3& rhs);
    };
    Emitter& operator <<(Emitter& out, const glm::mat3& v);

    // Mat4.
    template<>
    struct convert<glm::mat4>
    {
        static Node encode(const glm::mat4& rhs);
        static bool decode(const Node& node, glm::mat4& rhs);
    };
    Emitter& operator <<(Emitter& out, const glm::mat4& v);

    // Vec2.
    template<>
    struct convert<glm::vec2>
    {
        static Node encode(const glm::vec2& rhs);
        static bool decode(const Node& node, glm::vec2& rhs);
    };
    Emitter& operator <<(Emitter& out, const glm::vec2& v);

    // Vec3.
    template<>
    struct convert<glm::vec3>
    {
        static Node encode(const glm::vec3& rhs);
        static bool decode(const Node& node, glm::vec3& rhs);
    };
    Emitter& operator <<(Emitter& out, const glm::vec3& v);

    // Vec4.
    template<>
    struct convert<glm::vec4>
    {
        static Node encode(const glm::vec4& rhs);
        static bool decode(const Node& node, glm::vec4& rhs);
    };
    Emitter& operator <<(Emitter& out, const glm::vec4& v);

    // String-mat4 pair.
    template<>
    struct convert<std::pair<std::string, glm::mat4>>
    {
        static Node encode(const std::pair<std::string, glm::mat4>& rhs);
        static bool decode(const Node& node, std::pair<std::string, glm::mat4>& rhs);
    };
    Emitter& operator <<(Emitter& out, const std::pair<std::string, glm::mat4>& v);

    // Easy key-value writing.
    template<typename KeyT, typename ValueT>
    void KeyValue(Emitter& root, KeyT key, ValueT value)
    {
        root << Key << key << Value << value;
    }

    // Write a bunch of values for a single key.
    template<typename KeyT, typename ValueT>
    void KeyValueVec(Emitter& root, KeyT key, std::vector<ValueT> values)
    {
        root << Key << key << Value << BeginSeq;
        for (auto& val : values)
        {
            root << val;
        }
        root << EndSeq;
    }

}