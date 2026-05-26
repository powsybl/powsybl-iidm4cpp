/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/DynamicModelInfoAdder.hpp>

#include <memory>

#include <powsybl/iidm/Identifiable.hpp>
#include <powsybl/iidm/extensions/iidm/DynamicModelInfo.hpp>
#include <powsybl/stdcxx/make_unique.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

DynamicModelInfoAdder::DynamicModelInfoAdder(Extendable& extendable) :
    ExtensionAdder(extendable) {
}

std::unique_ptr<Extension> DynamicModelInfoAdder::createExtension(Extendable& extendable) const {
    if (stdcxx::isInstanceOf<Identifiable>(extendable)) {
        return std::unique_ptr<DynamicModelInfo>(new DynamicModelInfo(dynamic_cast<Identifiable&>(extendable), m_modelName));
    }
    throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable), stdcxx::demangle<Identifiable>()));
}

DynamicModelInfoAdder& DynamicModelInfoAdder::setModelName(const std::string& modelName) {
    m_modelName = modelName;
    return *this;
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
