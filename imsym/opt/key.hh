/* Copyright (C) Basemap, Inc DBA Automaton  All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Asa Hammond <asa@automaton.is>, 2021
 */
#pragma once
#include "common/struct.hh"

namespace imsym::key {

/*
 *  a la sym::Key
 */
struct key_t {
    using letter_t = char;
    using subscript_t = int64_t;
    using superscript_t = int64_t;
    static constexpr letter_t kInvalidLetter = static_cast<letter_t>(0);
    static constexpr subscript_t kInvalidSub = std::numeric_limits<subscript_t>::min();
    static constexpr superscript_t kInvalidSuper = std::numeric_limits<superscript_t>::min();

    char letter = kInvalidLetter;
    subscript_t sub = kInvalidSub;
    superscript_t super = kInvalidSuper;
};

}   // namespace imsym::key

COMMON_STRUCT_HASH(imsym::key, key_t, letter, sub, super);

