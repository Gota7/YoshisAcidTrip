#pragma once

#include "byml/types/array.hpp"
#include "byml/types/bool.hpp"
#include "byml/types/float.hpp"
#include "byml/types/int.hpp"
#include "byml/types/string.hpp"
#include "byml/types/stringHash.hpp"
#include "byml/types/stringTable.hpp"

// BYML file.
class BYML
{
    JPtr<BYMLNode> rootNode;
public:

    // Make an empty file.
    BYML() {}

    // Read a BYML from a stream.
    BYML(BStream& src);

    // Write archive to a stream.
    void Write(BStream& dst);

    // Write to YAML file.
    void WriteYAML(const JResPath& path);

};