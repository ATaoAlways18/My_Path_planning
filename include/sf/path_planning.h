#pragma once

// Main header file for Path Planning Library
// Include all algorithms in one convenient header

#include "graph.h"
#include "uninformed_search.h"
#include "informed_search.h"
#include "dynamic_search.h"
#include "sampling_based.h"

namespace sf {

// Convenience namespace aliases
namespace uninformed = sf;
namespace informed = sf;
namespace dynamic = sf;
namespace sampling = sf;

} // namespace sf
