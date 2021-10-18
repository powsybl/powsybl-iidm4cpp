/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/entsoe/XnodeAdder.hpp>

#include <powsybl/iidm/DanglingLine.hpp>
#include <powsybl/iidm/extensions/entsoe/Xnode.hpp>
#include <powsybl/stdcxx/format.hpp>
#include <powsybl/stdcxx/instanceof.hpp>
#include <powsybl/stdcxx/make_unique.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace entsoe {

XnodeAdder::XnodeAdder(Extendable& extendable) :
    ExtensionAdder(extendable) {
}

std::unique_ptr<Extension> XnodeAdder::createExtension(Extendable& extendable) {
    if (stdcxx::isInstanceOf<DanglingLine>(extendable)) {
        return std::unique_ptr<Xnode>(new Xnode(dynamic_cast<DanglingLine&>(extendable), m_code));
    }
    throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable), stdcxx::demangle<DanglingLine>()));
}

XnodeAdder& XnodeAdder::withCode(const std::string& code) {
    m_code = code;
    return *this;
}

}  // namespace entsoe

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
