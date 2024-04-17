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
#include "imsym/values_ops.hh"

#include <fmt/core.h>

// explicitly define the formatters for the imsym uses of flex vector
// when this is generalized, fmt/ranges kicks in. and blows up
// symforce pulls in fmt/ranges in a few places, so they compete.
// really should limit symforces use of fmt/ranges
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
struct fmt::formatter<immer::flex_vector<size_t>> {
    static constexpr auto parse(format_parse_context& ctx) {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto format(const immer::flex_vector<size_t>& v, FormatContext& ctx) {
        fmt::format_to(ctx.out(), "[");
        for (const auto& e : v) {
            fmt::format_to(ctx.out(), "{} ", e);
        }
        return fmt::format_to(ctx.out(), "]");
    }
};

template<>
struct fmt::formatter<immer::flex_vector<long>> {
    static constexpr auto parse(format_parse_context& ctx) {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto format(const immer::flex_vector<long>& v, FormatContext& ctx) {
        fmt::format_to(ctx.out(), "[");
        for (const auto& e : v) {
            fmt::format_to(ctx.out(), "{} ", e);
        }
        return fmt::format_to(ctx.out(), "]");
    }
};

template<>
struct fmt::formatter<immer::flex_vector<unsigned int>> {
    static constexpr auto parse(format_parse_context& ctx) {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto format(const immer::flex_vector<unsigned int>& v, FormatContext& ctx) {
        fmt::format_to(ctx.out(), "[");
        for (const auto& e : v) {
            fmt::format_to(ctx.out(), "{} ", e);
        }
        return fmt::format_to(ctx.out(), "]");
    }
};
template<>
struct fmt::formatter<immer::flex_vector<int>> {
    static constexpr auto parse(format_parse_context& ctx) {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto format(const immer::flex_vector<int>& v, FormatContext& ctx) {
        fmt::format_to(ctx.out(), "[");
        for (const auto& e : v) {
            fmt::format_to(ctx.out(), "{} ", e);
        }
        return fmt::format_to(ctx.out(), "]");
    }
};

template<>
struct fmt::formatter<immer::flex_vector<std::string>> {
    static constexpr auto parse(format_parse_context& ctx) {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto format(const immer::flex_vector<std::string>& v, FormatContext& ctx) {
        fmt::format_to(ctx.out(), "[");
        for (const auto& e : v) {
            fmt::format_to(ctx.out(), "{} ", e);
        }
        return fmt::format_to(ctx.out(), "]");
    }
};
template<>
struct fmt::formatter<immer::vector<float>> {
    static constexpr auto parse(format_parse_context& ctx) {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto format(const immer::vector<float>& v, FormatContext& ctx) {
        fmt::format_to(ctx.out(), "[");
        for (const auto& e : v) {
            fmt::format_to(ctx.out(), "{} ", e);
        }
        return fmt::format_to(ctx.out(), "]");
    }
};

template<>
struct fmt::formatter<immer::vector<double>> {
    static constexpr auto parse(format_parse_context& ctx) {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto format(const immer::vector<double>& v, FormatContext& ctx) {
        fmt::format_to(ctx.out(), "[");
        for (const auto& e : v) {
            fmt::format_to(ctx.out(), "{} ", e);
        }
        return fmt::format_to(ctx.out(), "]");
    }
};

template<>
struct fmt::formatter<immer::vector<size_t>> {
    static constexpr auto parse(format_parse_context& ctx) {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto format(const immer::vector<size_t>& v, FormatContext& ctx) {
        fmt::format_to(ctx.out(), "[");
        for (const auto& e : v) {
            fmt::format_to(ctx.out(), "{} ", e);
        }
        return fmt::format_to(ctx.out(), "]");
    }
};

template<>
struct fmt::formatter<immer::vector<long>> {
    static constexpr auto parse(format_parse_context& ctx) {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto format(const immer::vector<long>& v, FormatContext& ctx) {
        fmt::format_to(ctx.out(), "[");
        for (const auto& e : v) {
            fmt::format_to(ctx.out(), "{} ", e);
        }
        return fmt::format_to(ctx.out(), "]");
    }
};

template<>
struct fmt::formatter<immer::vector<unsigned int>> {
    static constexpr auto parse(format_parse_context& ctx) {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto format(const immer::vector<unsigned int>& v, FormatContext& ctx) {
        fmt::format_to(ctx.out(), "[");
        for (const auto& e : v) {
            fmt::format_to(ctx.out(), "{} ", e);
        }
        return fmt::format_to(ctx.out(), "]");
    }
};
template<>
struct fmt::formatter<immer::vector<int>> {
    static constexpr auto parse(format_parse_context& ctx) {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto format(const immer::vector<int>& v, FormatContext& ctx) {
        fmt::format_to(ctx.out(), "[");
        for (const auto& e : v) {
            fmt::format_to(ctx.out(), "{} ", e);
        }
        return fmt::format_to(ctx.out(), "]");
    }
};

template<>
struct fmt::formatter<immer::vector<std::string>> {
    static constexpr auto parse(format_parse_context& ctx) {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto format(const immer::vector<std::string>& v, FormatContext& ctx) {
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
    auto format(const imsym::values::valuesd_t& vals, FormatContext& ctx) {
        fmt::format_to(ctx.out(), "values\n");
        const auto full_index =
            create_index(vals, keys<double>(vals.map, true));   // sort by offset
                                                                //
        for (const auto& entry : full_index.entries) {
            fmt::format_to(ctx.out(),
                           "\n\t{} {}",
                           entry,
                           vals.data.drop(entry.offset).take(entry.storage_dim));
        }

        return ctx.out();
    }
};

template<>
struct fmt::formatter<immer::flex_vector<imsym::key::key_t>> {
    static constexpr auto parse(format_parse_context& ctx) {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto format(const immer::flex_vector<imsym::key::key_t>& v, FormatContext& ctx) {
        fmt::format_to(ctx.out(), "[");
        for (const auto& e : v) {
            fmt::format_to(ctx.out(), "{} ", e);
        }
        return fmt::format_to(ctx.out(), "]");
    }
};

template<>
struct fmt::formatter<immer::vector<imsym::key::key_t>> {
    static constexpr auto parse(format_parse_context& ctx) {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto format(const immer::vector<imsym::key::key_t>& v, FormatContext& ctx) {
        fmt::format_to(ctx.out(), "[");
        for (const auto& e : v) {
            fmt::format_to(ctx.out(), "{} ", e);
        }
        return fmt::format_to(ctx.out(), "]");
    }
};

template<>
struct fmt::formatter<const std::optional<long>&> {
    static constexpr auto parse(format_parse_context& ctx) {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto format(const std::optional<long>& v, FormatContext& ctx) {
        if (v) {
            return fmt::format_to(ctx.out(), v.value());
        } else {
            return fmt::format_to(ctx.out(), "{}");
        }
    }
};

template<typename T>
struct fmt::formatter<const std::optional<T>&> {
    static constexpr auto parse(format_parse_context& ctx) {
        return ctx.begin();
    }

    template<typename FormatContext, typename V, typename = std::enable_if_t<std::is_integral_v<T>>>
    auto format(const std::optional<V>& v, FormatContext& ctx) {
        if (v) {
            return fmt::format_to(ctx.out(), v.value());
        } else {
            return fmt::format_to(ctx.out(), "{}");
        }
    }
};

