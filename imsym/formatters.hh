#pragma once

//
#include <immer/flex_vector.hpp>
#include <immer/map.hpp>
#include <immer/vector.hpp>
//
#include "common/cereal/immer_flex_vector.hh"
#include "common/cereal/immer_map.hh"
#include "common/cereal/immer_vector.hh"
//
#include "imsym/key.hh"
#include "imsym/values.hh"

#include <fmt/core.h>

template<>
struct fmt::formatter<immer::flex_vector<float>> {
    static constexpr auto parse(format_parse_context& ctx) {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto format(const immer::flex_vector<float>& v, FormatContext& ctx) {
        fmt::format_to(ctx.out(), "[");
        for (const auto& e : v) {
            fmt::format_to(ctx.out(), "{} ", e);
        }
        return fmt::format_to(ctx.out(), "]");
    }
};

template<>
struct fmt::formatter<immer::flex_vector<double>> {
    static constexpr auto parse(format_parse_context& ctx) {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto format(const immer::flex_vector<double>& v, FormatContext& ctx) {
        fmt::format_to(ctx.out(), "[");
        for (const auto& e : v) {
            fmt::format_to(ctx.out(), "{} ", e);
        }
        return fmt::format_to(ctx.out(), "]");
    }
};

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
//
template<>
struct fmt::formatter<imsym::values::index_entry_t> {
    static constexpr auto parse(format_parse_context& ctx) {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto format(const imsym::values::index_entry_t& i, FormatContext& ctx) {
        return fmt::format_to(
            ctx.out(),
            "index_entry_t: key: {} offset: {} storage_dim: {} tangent_dim: {} type: {}",
            i.key,
            i.offset,
            i.storage_dim,
            i.tangent_dim,
            i.type);
    }
};

template<>
struct fmt::formatter<imsym::values::index_t> {
    static constexpr auto parse(format_parse_context& ctx) {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto format(const imsym::values::index_t& i, FormatContext& ctx) {
        return fmt::format_to(ctx.out(),
                              "index_t: {} storage_dim: {} tangent_dim: {} entries: {}",
                              i.storage_dim,
                              i.tangent_dim,
                              i.entries);
    }
};

template<>
struct fmt::formatter<imsym::values::valuesd_t> {
    static constexpr auto parse(format_parse_context& ctx) {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto format(const imsym::values::valuesd_t& values, FormatContext& ctx) {
        fmt::format_to(ctx.out(), "values\n");

        const auto full_index =
            create_index(values, keys<double>(values.map, true));   // sort by offset
                                                                    //
        for (const auto& entry : full_index.entries) {
            fmt::format_to(ctx.out(),
                           "\n\t{} {}",
                           entry,
                           values.data.drop(entry.offset).take(entry.storage_dim));
        }

        return ctx.out();
    }
};

