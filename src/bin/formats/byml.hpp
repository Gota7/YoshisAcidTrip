#pragma once

#include "byml/types/array.hpp"
#include "byml/types/bin.hpp"
#include "byml/types/bool.hpp"
#include "byml/types/double.hpp"
#include "byml/types/file.hpp"
#include "byml/types/float.hpp"
#include "byml/types/hash.hpp"
#include "byml/types/int.hpp"
#include "byml/types/int64.hpp"
#include "byml/types/uint.hpp"
#include "byml/types/uint64.hpp"
#include "byml/types/string.hpp"
#include "byml/types/stringHash.hpp"
#include "byml/types/stringTable.hpp"

// BYML file.
class BYML
{
public:
    JPtr<BYMLNode> rootNode;

    // Make an empty file.
    BYML() {}

    // Read a BYML from a stream.
    BYML(BStream& src);

    // Write archive to a stream.
    void Write(BStream& dst);

    // Write to YAML file. Writes how many external files were written.
    int WriteYAML(const JResPath& path);

};