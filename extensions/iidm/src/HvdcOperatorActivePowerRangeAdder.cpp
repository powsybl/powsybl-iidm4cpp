/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/HvdcOperatorActivePowerRangeAdder.hpp>

#include <powsybl/iidm/HvdcLine.hpp>
#include <powsybl/iidm/extensions/iidm/HvdcOperatorActivePowerRange.hpp>
#include <powsybl/stdcxx/format.hpp>
#include <powsybl/stdcxx/memory.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

HvdcOperatorActivePowerRangeAdder::HvdcOperatorActivePowerRangeAdder(Extendable& extendable) :
    ExtensionAdder(extendable) {
}

std::unique_ptr<Extension> HvdcOperatorActivePowerRangeAdder::createExtension(Extendable& extendable) const {
    if (stdcxx::isInstanceOf<HvdcLine>(extendable)) {
        return std::unique_ptr<HvdcOperatorActivePowerRange>(new HvdcOperatorActivePowerRange(dynamic_cast<HvdcLine&>(extendable), m_oprFromCS1toCS2, m_oprFromCS2toCS1));
    }
    throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable), stdcxx::demangle<HvdcLine>()));
}

HvdcOperatorActivePowerRangeAdder& HvdcOperatorActivePowerRangeAdder::withOprFromCS1toCS2(double oprFromCS1toCS2) {
    m_oprFromCS1toCS2 = oprFromCS1toCS2;
    return *this;
}

HvdcOperatorActivePowerRangeAdder& HvdcOperatorActivePowerRangeAdder::withOprFromCS2toCS1(double oprFromCS2toCS1) {
    m_oprFromCS2toCS1 = oprFromCS2toCS1;
    return *this;
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
