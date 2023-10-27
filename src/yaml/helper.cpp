#include "helper.hpp"

YAML::Node YAML::convert<glm::mat3>::encode(const glm::mat3& rhs)
{
    Node node;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            node.push_back(rhs[i][j]);
        }
    }
    return node;
}

bool YAML::convert<glm::mat3>::decode(const Node& node, glm::mat3& rhs)
{
    if(!node.IsSequence() || node.size() != 9) return false;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            rhs[i][j] = node[i * 3 + j].as<float>();
        }
    }
    return true;
}

YAML::Emitter& YAML::operator<<(YAML::Emitter& out, const glm::mat3& v)
{
    out << YAML::Flow << YAML::BeginSeq;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            out << v[i][j];
        }
    }
    out << YAML::EndSeq;
	return out;
}

YAML::Node YAML::convert<glm::mat4>::encode(const glm::mat4& rhs)
{
    Node node;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            node.push_back(rhs[i][j]);
        }
    }
    return node;
}

bool YAML::convert<glm::mat4>::decode(const Node& node, glm::mat4& rhs)
{
    if(!node.IsSequence() || node.size() != 16) return false;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            rhs[i][j] = node[i * 4 + j].as<float>();
        }
    }
    return true;
}

YAML::Emitter& YAML::operator<<(YAML::Emitter& out, const glm::mat4& v)
{
    out << YAML::Flow << YAML::BeginSeq;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            out << v[i][j];
        }
    }
    out << YAML::EndSeq;
	return out;
}

YAML::Node YAML::convert<glm::vec2>::encode(const glm::vec2& rhs)
{
    Node node;
    node.push_back(rhs.x);
    node.push_back(rhs.y);
    return node;
}

bool YAML::convert<glm::vec2>::decode(const Node& node, glm::vec2& rhs)
{
    if(!node.IsSequence() || node.size() != 2) return false;
    rhs.x = node[0].as<float>();
    rhs.y = node[1].as<float>();
    return true;
}

YAML::Emitter& YAML::operator<<(YAML::Emitter& out, const glm::vec2& v)
{
    out << YAML::Flow << YAML::BeginSeq;
    out << v.x;
    out << v.y;
    out << YAML::EndSeq;
	return out;
}

YAML::Node YAML::convert<glm::vec3>::encode(const glm::vec3& rhs)
{
    Node node;
    node.push_back(rhs.x);
    node.push_back(rhs.y);
    node.push_back(rhs.z);
    return node;
}

bool YAML::convert<glm::vec3>::decode(const Node& node, glm::vec3& rhs)
{
    if(!node.IsSequence() || node.size() != 3) return false;
    rhs.x = node[0].as<float>();
    rhs.y = node[1].as<float>();
    rhs.z = node[2].as<float>();
    return true;
}

YAML::Emitter& YAML::operator<<(YAML::Emitter& out, const glm::vec3& v)
{
    out << YAML::Flow << YAML::BeginSeq;
    out << v.x;
    out << v.y;
    out << v.z;
    out << YAML::EndSeq;
	return out;
}

YAML::Node YAML::convert<glm::vec4>::encode(const glm::vec4& rhs)
{
    Node node;
    node.push_back(rhs.x);
    node.push_back(rhs.y);
    node.push_back(rhs.z);
    node.push_back(rhs.w);
    return node;
}

bool YAML::convert<glm::vec4>::decode(const Node& node, glm::vec4& rhs)
{
    if(!node.IsSequence() || node.size() != 4) return false;
    rhs.x = node[0].as<float>();
    rhs.y = node[1].as<float>();
    rhs.z = node[2].as<float>();
    rhs.w = node[3].as<float>();
    return true;
}

YAML::Emitter& YAML::operator<<(YAML::Emitter& out, const glm::vec4& v)
{
    out << YAML::Flow << YAML::BeginSeq;
    out << v.x;
    out << v.y;
    out << v.z;
    out << v.w;
    out << YAML::EndSeq;
	return out;
}

YAML::Node YAML::convert<std::pair<std::string, glm::mat4>>::encode(const std::pair<std::string, glm::mat4>& rhs)
{
    Node node;
    node.push_back(rhs.first);
    node.push_back(rhs.second);
    return node;
}

bool YAML::convert<std::pair<std::string, glm::mat4>>::decode(const Node& node, std::pair<std::string, glm::mat4>& rhs)
{
    if(!node.IsSequence() || node.size() != 2) return false;
    rhs.first = node[0].as<std::string>();
    rhs.second = node[1].as<glm::mat4>();
    return true;
}

YAML::Emitter& YAML::operator<<(YAML::Emitter& out, const std::pair<std::string, glm::mat4>& v)
{
    out << YAML::Flow << YAML::BeginSeq;
    out << v.first << v.second;
    out << YAML::EndSeq;
	return out;
}