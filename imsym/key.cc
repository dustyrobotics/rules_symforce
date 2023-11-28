/* Copyright (C) Basemap, Inc DBA Automaton  All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Asa Hammond <asa@automaton.is>, 2021
 */
#include "imsym/imsym.hh"

namespace imsym::key {

// return time in ns
auto get_t(const key_t& k) -> size_t { return static_cast<size_t>(k.sub); };
// TODO what about binary factors?

}  // namespace imsym::key
