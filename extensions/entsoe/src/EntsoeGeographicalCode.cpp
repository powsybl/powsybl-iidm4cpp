/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/entsoe/EntsoeGeographicalCode.hpp>

#include <powsybl/iidm/Enum.hpp>
#include <powsybl/stdcxx/export.hpp>

namespace powsybl {

namespace iidm {

namespace Enum {

template <>
IIDM_DECLSPEC const std::initializer_list<std::string>& getNames<powsybl::iidm::extensions::entsoe::EntsoeGeographicalCode>() {
    static std::initializer_list<std::string> s_names = {
        "AL",
        "AT",
        "BA",
        "BE",
        "BG",
        "BY",
        "CH",
        "CZ",
        "DE",
        "D1",
        "D2",
        "D4",
        "D7",
        "D8",
        "DK",
        "ES",
        "FR",
        "GB",
        "GR",
        "HR",
        "HU",
        "IT",
        "LU",
        "LT",
        "MA",
        "MD",
        "ME",
        "MK",
        "NL",
        "NO",
        "PL",
        "PT",
        "RO",
        "RS",
        "RU",
        "SE",
        "SK",
        "SI",
        "TR",
        "UA",
        "UC",
        "UX"
    };
    return s_names;
}

}  // namespace Enum

}  // namespace iidm

}  // namespace powsybl

