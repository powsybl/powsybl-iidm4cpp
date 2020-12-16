/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_STDCXX_MAP_HXX
#define POWSYBL_STDCXX_MAP_HXX

#include <powsybl/stdcxx/map.hpp>

namespace stdcxx {

template <typename Key, typename Value>
Value getOrDefault(const std::map<Key, Value>& map, const Key& key, const Value& defaultValue) {
    auto it = map.find(key);
    return it != map.end() ? it->second : defaultValue;
}

}  // namespace stdcxx

#endif  // POWSYBL_STDCXX_MAP_HXX
