/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "NetworkSourceExt.hpp"

#include <powsybl/iidm/Network.hpp>
#include <powsybl/stdcxx/demangle.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

namespace extensions {

NetworkSourceExt::NetworkSourceExt(Network& network, const std::string& sourceData) :
    Extension(network),
    m_sourceData(sourceData) {
}

void NetworkSourceExt::assertExtendable(const stdcxx::Reference<Extendable>& extendable) const {
    if (extendable && !stdcxx::isInstanceOf<Network>(extendable.get())) {
        throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable.get()), stdcxx::demangle<Network>()));
    }
}

const std::string& NetworkSourceExt::getName() const {
    static std::string s_name = "networkSource";
    return s_name;
}

const std::type_index& NetworkSourceExt::getType() const {
    static std::type_index s_type = typeid(NetworkSourceExt);
    return s_type;
}

const std::string& NetworkSourceExt::getSourceData() const {
    return m_sourceData;
}

}  // namespace extensions

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl
