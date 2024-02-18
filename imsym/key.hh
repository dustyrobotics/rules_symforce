/* Copyright (C) Basemap, Inc DBA Automaton  All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Asa Hammond <asa@automaton.is>, 2021
 */
#pragma once
#include "common/struct.hh"

#include <fmt/core.h>

#include <variant>

/*
 * immutable variant of symforce structures for use in logging and playback
 * they are hashable and convertable to their underlying symforce type to be used in symforce
 * structures. It may be possible to use these as the underlying types in sym::Values in an
 * optimizer of the future by using c++ Concepts.
 */
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

    /*
    key_t() {
    }

    key_t(letter_t _letter) {
        letter = _letter;
    }

    key_t(letter_t _letter, subscript_t _sub) {
        letter = _letter;
        sub = _sub;
    }

    key_t(letter_t _letter, subscript_t _sub, superscript_t _super) {
        letter = _letter;
        sub = _sub;
        super = _super;
    }
    */
};

}   // namespace imsym::key

template<>
struct fmt::formatter<imsym::key::key_t> {
    static constexpr auto parse(format_parse_context& ctx) {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto format(const imsym::key::key_t& k, FormatContext& ctx) {
        using imsym::key::key_t;
        if (k.sub == key_t::kInvalidSub and k.super == key_t::kInvalidSuper) {
            return fmt::format_to(ctx.out(), "{}", k.letter);
        }
        if (k.sub != key_t::kInvalidSub and k.super == key_t::kInvalidSuper) {
            return fmt::format_to(ctx.out(), "{}(t:{}) ", k.letter, k.sub);
        }
        return fmt::format_to(ctx.out(), "{} sub {} super {}", k.letter, k.sub, k.super);
    }
};

COMMON_STRUCT_HASH(imsym::key, key_t, letter, sub, super);
