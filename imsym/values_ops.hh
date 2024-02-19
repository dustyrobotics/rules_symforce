/* Copyright (C) Basemap, Inc DBA Automaton  All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Asa Hammond <asa@automaton.is>, 2021
 */

#pragma once
#include "common/cereal/immer_flex_vector.hh"
#include "common/cereal/immer_map.hh"
#include "common/cereal/immer_vector.hh"
#include "common/immer/utils.hh"
#include "common/struct.hh"
//
#include "imsym/values.hh"

//
#include <immer/flex_vector.hpp>
#include <immer/map.hpp>
#include <immer/vector.hpp>
#include <lager/util.hpp>

#include <vector>

namespace imsym::values {

using std::move;

template<typename Scalar>
inline auto remove(values_t<Scalar> values, imsym::key::key_t key) -> values_t<Scalar> {
    values.map = move(values.map).erase(key);
    // leave data alone, it can be cleaned up manually later with the Cleanup() method
    return values;
};

/**
 * Create an index from the given ordered subset of keys. This object can then be used
 * for repeated efficient operations on that subset of keys.
 *
 * If you want an index of all the keys, call `values.CreateIndex(values.Keys())`.
 *
 * An index will be INVALIDATED if the following happens:
 *  1) Remove() is called with a contained key, or RemoveAll() is called
 *  2) Cleanup() is called to re-pack the data array
 *
 * NOTE(hayk): We could also add a simple UpdateIndex(&index) method, since the offset is the
 * only thing that needs to get updated after repacking.
 */

template<typename Scalar>
inline auto create_index(const values_t<Scalar>& values,
                         const immer::vector<imsym::key::key_t>& keys) -> index_t {
    index_t index{.storage_dim = 0, .tangent_dim = 0};

    for (const auto& key : keys) {
        const auto& val = values.map.find(key);

        if (val == nullptr) {
            // REALLY DONT WANT THIS TO THROW!!
            throw std::runtime_error("Tried to create index for key not in values");
            //     fmt::format("Tried to create index for key {} not in values", key));
        }

        const auto entry = *val;
        index.entries = move(index.entries).push_back(entry);
        index.storage_dim += entry.storage_dim;
        index.tangent_dim += entry.tangent_dim;
    }

    return index;
};

// ----------------------------------------------------------------------------
// Public Methods
// ----------------------------------------------------------------------------
template<typename Scalar>
auto has(const values_t<Scalar>& values, const key::key_t& key) -> bool {
    return values.map.count(key);
}

template<typename Scalar>
auto find_letter_sub(const values_t<Scalar>& values, const key::key_t& key)
    -> std::optional<key::key_t> {
    for (const auto& [k, v] : values.map) {
        if (k.letter == key.letter and k.sub == key.sub) {
            return k;
        }
    }
    return {};
}

// need a storageOps that can read immer::vector

template<typename Scalar, typename T>
auto at(const values_t<Scalar>& values, const index_entry_t& entry) -> T {
    // Check the type
    const sym::type_t type = sym::StorageOps<T>::TypeEnum();

    if (entry.type != type) {
        throw std::runtime_error("Mismatched types; index entry is wrong type ");
        // fmt::format("Mismatched types; index entry is type {}, T is {}", entry.type, type));
    }

    auto slice = values.data.drop(entry.offset).take(entry.storage_dim);
    if (slice.size() < entry.tangent_dim) {
        // TODO really don't want this to throw
        throw std::runtime_error("not enough data to load data");
        // fmt::format("not enough data {} {} to load data", slice.size(), entry.storage_dim));
    }

    // slice off the bit of entry
    const auto data = common::immer::to_std(slice);
    // Construct the object
    return sym::StorageOps<T>::FromStorage(data.data());
}

template<typename Scalar, typename T>
auto at(const values_t<Scalar>& values, const key::key_t& key) -> T {
    auto entry = values.map.at(key);
    // TODO handle failure
    return at<T>(values, entry);
}

template<typename T>
auto at(const valuesd_t& values, const index_entry_t& entry) -> T {
    return at<double, T>(values, entry);
}

template<typename T>
auto at(const valuesf_t& values, const index_entry_t& entry) -> T {
    return at<float, T>(values, entry);
}

template<typename T>
auto at(const valuesd_t& values, const key::key_t& key) -> T {
    return at<double, T>(values, key);
}

template<typename T>
auto at(const valuesf_t& values, const key::key_t& key) -> T {
    return at<float, T>(values, key);
}

template<typename Scalar>
inline auto keys(const values_t<Scalar>& values) {
    return keys<Scalar>(values.map);
};

template<typename Scalar>
inline auto keys(typename values_t<Scalar>::map_t map, const bool sort_by_offset = true) {
    // Sort the keys by offset so iterating through is saner and more memory friendly
    if (sort_by_offset) {
        std::vector<imsym::key::key_t> keys;
        keys.reserve(map.size());

        for (const auto& [k, v] : map) {
            keys.push_back(k);
        }
        std::sort(keys.begin(), keys.end(), [&](const auto& a, const auto& b) {
            return map.at(a).offset < map.at(b).offset;
        });
        return common::immer::vector_from_std(keys);
    }

    immer::vector<imsym::key::key_t> keys;
    for (const auto& [k, v] : map) {
        keys = move(keys).push_back(k);
    }
    return keys;
};

/**
 * Repack the data array to get rid of empty space from removed keys. If regularly removing
 * keys, it's up to the user to call this appropriately to avoid storage growth. Returns the
 * number of Scalar elements cleaned up from the data array.
 *
 * It will INVALIDATE all indices, offset increments, and pointers.
 * Re-create an index with CreateIndex().
 */
template<typename Scalar>
inline auto cleanup(values_t<Scalar> values) -> std::pair<decltype(values), size_t> {
    // Copy the original data
    const auto data_copy = values.data;

    // Build an index of all keys
    const auto full_index = create_index(values, keys<Scalar>(values));

    // initialize all the data
    values.data = immer::vector<Scalar>(full_index.storage_dim, Scalar{});

    assert(data_copy.size() >= values.data.size());

    // Copy into new data and update the offset in the map
    int32_t new_offset = 0;
    for (index_entry_t entry : full_index.entries) {
        // copy data over
        for (int i = entry.offset, j = new_offset; i < (entry.offset + entry.storage_dim);
             i++, j++) {
            values.data = move(values.data).set(j, data_copy.at(i));
        }

        entry.offset = new_offset;
        values.map = move(values.map).set(entry.key, entry);

        new_offset += entry.storage_dim;
    }
    return std::pair<decltype(values), size_t>{values, data_copy.size() - values.data.size()};
};

template<typename Scalar>
inline auto merge(const values_t<Scalar>& a, const values_t<Scalar>& b) -> values_t<Scalar> {
    auto out = a;
    for (const auto& [k, v] : b.map) {
        auto v_out = v;
        // UPDATE THE POSITION OF THE ENTRY by length of a.data
        // we are appending all the data from b to a
        v_out.offset += a.data.size();
        out.map = move(out.map).set(k, v_out);
    }

    for (const auto& v : b.data) {
        out.data = move(out.data).push_back(v);
    }
    return out;
}

template<typename Scalar>
inline auto merge(const values_t<Scalar>& a, const values_t<Scalar>& b, const values_t<Scalar>& c)
    -> values_t<Scalar> {
    return merge(merge(a, b), c);
}

/**
 * Efficiently update the keys from a different structured values, given by
 * `index_a` and `index_b`. This purely copies slices of the data arrays.
 *
 * `index_a` MUST be valid for this object; `index_b` MUST be valid for other object.
 */
template<typename Scalar>
inline values_t<Scalar> update(const index_t& index_a,
                               const index_t& index_b,
                               const values_t<Scalar>& values_a,
                               const values_t<Scalar>& values_b) {
    auto values_out = values_a;
    assert(index_a.entries.size() == index_b.entries.size());
    for (int i = 0; i < static_cast<int>(index_a.entries.size()); ++i) {
        const imsym::values::index_entry_t& entry_a = index_a.entries[i];
        const imsym::values::index_entry_t& entry_b = index_b.entries[i];
        assert(entry_a.storage_dim == entry_b.storage_dim);
        assert(entry_a.key == entry_b.key);

        auto pre_a = values_a.data.take(entry_a.offset);
        auto post_a = values_a.data.drop(entry_a.offset + entry_a.storage_dim);

        auto slice_b = values_b.data.drop(entry_b.offset).take(entry_b.storage_dim);

        values_out.data = pre_a + slice_b + post_a;
    }
    return values_out;
}

/*
 * update a single key from other
 * this avoids having to know about type info
 * key doesn't exist on the a side
 */
template<typename Scalar>
inline values_t<Scalar> copy_data_for_new_key(const values_t<Scalar>& values_a,
                                              const values_t<Scalar>& values_b,
                                              const imsym::key::key_t& key) {
    auto values_out = values_a;
    auto entry_b = values_b.map.at(key);

    // the new data to insert
    const auto slice_b = values_b.data.drop(entry_b.offset).take(entry_b.storage_dim);
    entry_b.offset = values_out.data.size();
    values_out.data = move(values_out.data) + slice_b;
    values_out.map = move(values_out.map).set(key, entry_b);

    return values_out;
}

// data has to be the same size for both sides
// we just insert the data directly over the old data
// key has to exist on both sides
template<typename Scalar>
inline values_t<Scalar> copy_data_for_existing_key(const values_t<Scalar>& values_a,
                                                   const values_t<Scalar>& values_b,
                                                   const imsym::key::key_t& key) {
    auto values_out = values_a;

    const auto& entry_a = values_a.map.at(key);
    const auto& entry_b = values_b.map.at(key);
    assert(entry_a.storage_dim == entry_b.storage_dim);
    assert(entry_a.key == entry_b.key);

    auto pre_a = values_a.data.take(entry_a.offset);
    auto post_a = values_a.data.drop(entry_a.offset + entry_a.storage_dim);
    auto slice_b = values_b.data.drop(entry_b.offset).take(entry_b.storage_dim);

    values_out.data = pre_a + slice_b + post_a;

    return values_out;
}
/*
 * update a single key from other
 * this avoids having to know about type info
 * if key exists on both sides, straightforward
 * if key exists in other
 * - copy the data from other
 */
template<typename Scalar>
inline values_t<Scalar>
    update(const values_t<Scalar>& values_a, const values_t<Scalar>& values_b, const key_t& key) {
    auto values_out = values_a;

    const auto& entry_a = values_a.map.get(key);
    const auto& entry_b = values_b.map.get(key);

    assert(entry_a.storage_dim == entry_b.storage_dim);
    assert(entry_a.key == entry_b.key);

    const auto pre_a = values_a.data.take(entry_a.offset);
    const auto post_a = values_a.data.drop(entry_a.offset + entry_a.storage_dim);

    // the new data to insert
    const auto slice_b = values_b.data.drop(entry_b.offset).take(entry_b.storage_dim);

    values_out.data = pre_a + slice_b + post_a;

    return values_out;
}
/**
 * extract values from the given keys into a new values
 */
template<typename Scalar>
inline values_t<Scalar> extract(const values_t<Scalar>& other, const index_t& index) {
    auto values = values_t<Scalar>{};

    for (const auto& other_entry : index.entries) {
        auto entry = other_entry;
        entry.offset = static_cast<int32_t>(values.data.size());

        const auto other_slice = other.data.drop(other_entry.offset).take(other_entry.storage_dim);

        values.data = values.data + other_slice;
        values.map = move(values.map).set(entry.key, entry);
    }
    return values;
}

template<typename Scalar>
values_t<Scalar>::values_t(
    std::initializer_list<std::tuple<imsym::key::key_t, AllowedTypes<Scalar>>> init_list) {
    values_t<Scalar> me = *this;
    for (const auto& [k, v] : init_list) {
        me = lager::match(v)([me = move(me), k = k](auto a) {
            return set(move(me), k, a);
        });
    }
    map = move(me.map);
    data = move(me.data);
};

}   // namespace imsym::values
// TODO move this to a sym archive file?
namespace cereal {
template<class Archive>
void save(Archive& archive, const sym::type_t& m) {
    archive(static_cast<int32_t>(m.value));
}

template<class Archive>
void load(Archive& archive, sym::type_t& m) {
    int32_t value;
    archive(value);
    m.value = static_cast<sym::type_t::option_t>(value);
}
}   // namespace cereal

namespace imsym::values {

// perhaps move this to a detail namespace

/*
 * check equality of map contents
 * this is different than object equality offered by operator == on the map which checks for
 * identity() through map tree operations
 */
inline auto contents_equal(const immer::map<imsym::key::key_t, imsym::values::index_entry_t>& a,
                           const immer::map<imsym::key::key_t, imsym::values::index_entry_t>& b)
    -> bool {
    for (const auto& [key, entry] : a) {
        if (b.count(key) == 0) {
            return false;
        }
        // entry exists in b
        const imsym::values::index_entry_t& v_b = b.at(key);
        if (entry != v_b) {
            return false;
        }
    }
    // check back in the other direction
    for (const auto& [key, entry] : b) {
        if (a.count(key) == 0) {
            return false;
        }
        // entry exists in a
        if (entry != a.at(key)) {
            return false;
        }
    }
    return true;
}

/* check equality of map and data
 * this is different than object equality offered by operator == on the map which checks for
 * identity()
 */
template<typename Scalar>
inline auto contents_equal(const values_t<Scalar>& a, const values_t<Scalar>& b) -> bool {
    return contents_equal(a.map, b.map) and (a.data == b.data);
}
}   // namespace imsym::values
