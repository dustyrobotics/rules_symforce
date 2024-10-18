#pragma once

#include "common/formatter/std.hh"
#include "imsym/opt/key.hh"
#include "imsym/opt/types.hh"
#include "imsym/opt/values.hh"
#include "imsym/opt/values_ops.hh"
#include "types.hh"

#include <fmt/core.h>
template<>
struct fmt::formatter<imsym::coords_t> {
    static constexpr auto parse(format_parse_context& ctx) {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto format(const imsym::coords_t& c, FormatContext& ctx) {
        return fmt::format_to(ctx.out(), "coords_t: row: {} col: {}", c.row, c.col);
    }
};

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
            create_index(vals, imsym::values::keys<double>(vals.map, true));   // sort by offset
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
struct fmt::formatter<immer::set<imsym::key::key_t>> {
    static constexpr auto parse(format_parse_context& ctx) {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto format(const immer::set<imsym::key::key_t>& v, FormatContext& ctx) {
        fmt::format_to(ctx.out(), "(");
        for (const auto& e : v) {
            fmt::format_to(ctx.out(), "{},", e);
        }
        return fmt::format_to(ctx.out(), ")");
    }
};

template<>
struct fmt::formatter<imsym::dense_matrix_t> {
    static constexpr auto parse(format_parse_context& ctx) {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto format(const imsym::dense_matrix_t& m, FormatContext& ctx) {
        fmt::format_to(ctx.out(), "dense_matrix_t.T [ {}\n", m.size);
        size_t data_idx = 0;
        for (size_t col_idx = 0; col_idx < m.size.col; col_idx++) {
            for (size_t row_idx = 0; row_idx < m.size.row; row_idx++) {
                auto val = m.data[data_idx++];
                fmt::format_to(ctx.out(), "{},", val);
            }
            fmt::format_to(ctx.out(), "\n");
        }
        return fmt::format_to(ctx.out(), "]");
    }
};

template<>
struct fmt::formatter<imsym::dense_lt_matrix_t> {
    static constexpr auto parse(format_parse_context& ctx) {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto format(const imsym::dense_lt_matrix_t& m, FormatContext& ctx) {
        fmt::format_to(ctx.out(), "dense_lt_matrix_t.T [ {}\n", m.size);
        size_t data_idx = 0;
        for (size_t col_idx = 0; col_idx < m.size.col; col_idx++) {
            for (size_t row_idx = col_idx; row_idx < m.size.row; row_idx++) {
                auto val = m.data[data_idx++];
                fmt::format_to(ctx.out(), "{},", val);
            }
            fmt::format_to(ctx.out(), "\n");
        }
        return fmt::format_to(ctx.out(), "]");
    }
};
