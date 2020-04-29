/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_STDCXX_MAP_HPP
#define POWSYBL_STDCXX_MAP_HPP

#include <map>

namespace stdcxx {

template <typename K, typename V>
class MapBuilder {
public:
    MapBuilder() = default;

    MapBuilder(const MapBuilder&) = default;

    MapBuilder(MapBuilder&&) noexcept = default;

    ~MapBuilder() = default;

    MapBuilder& operator=(const MapBuilder&) = default;

    MapBuilder& operator=(MapBuilder&&) noexcept = default;

    std::map<K, V> build() {
        return m_items;
    }

    MapBuilder& put(K&& key, V&& value) {
        m_items.emplace(std::make_pair(key, value));
        return *this;
    }

private:
    std::map<K, V> m_items;
};

}  // namespace stdcxx

#endif  // POWSYBL_STDCXX_MAP_HPP
