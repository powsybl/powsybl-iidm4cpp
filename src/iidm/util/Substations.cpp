/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/util/Substations.hpp>

#include <powsybl/iidm/Branch.hpp>
#include <powsybl/iidm/Connectable.hpp>
#include <powsybl/iidm/HvdcConverterStation.hpp>
#include <powsybl/iidm/HvdcLine.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/Terminal.hpp>
#include <powsybl/iidm/ThreeWindingsTransformer.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>

namespace powsybl {

namespace iidm {

namespace Substations {

bool areDifferentSubstations(const stdcxx::CReference<Substation>& s1, const Substation& s2) {
    return static_cast<bool>(s1) && !stdcxx::areSame(s1.get(), s2);
}

PowsyblException createIsolationException(const Substation& substation) {
    return PowsyblException(stdcxx::format("The substation %1% is still connected to another substation", substation.getId()));
}

void checkRemovability(const Substation& substation) {
    for (const VoltageLevel& vl : substation.getVoltageLevels()) {
        for (const Connectable& connectable : vl.getConnectables()) {
            if (stdcxx::isInstanceOf<Branch>(connectable)) {
                checkRemovability(substation, dynamic_cast<const Branch&>(connectable));
            } else if (stdcxx::isInstanceOf<ThreeWindingsTransformer>(connectable)) {
                checkRemovability(substation, dynamic_cast<const ThreeWindingsTransformer&>(connectable));
            } else if (stdcxx::isInstanceOf<HvdcConverterStation>(connectable)) {
                checkRemovability(substation, dynamic_cast<const HvdcConverterStation&>(connectable));
            }
        }
    }
}

void checkRemovability(const Substation& substation, const Branch& branch) {
    const auto& s1 = branch.getTerminal1().getVoltageLevel().getNullableSubstation();
    const auto& s2 = branch.getTerminal2().getVoltageLevel().getNullableSubstation();
    if (areDifferentSubstations(s1, substation) || areDifferentSubstations(s2, substation)) {
        throw createIsolationException(substation);
    }
}

void checkRemovability(const Substation& substation, const ThreeWindingsTransformer& twt) {
    const auto& s1 = twt.getLeg1().getTerminal().getVoltageLevel().getNullableSubstation();
    const auto& s2 = twt.getLeg2().getTerminal().getVoltageLevel().getNullableSubstation();
    const auto& s3 = twt.getLeg3().getTerminal().getVoltageLevel().getNullableSubstation();
    if (areDifferentSubstations(s1, substation) || areDifferentSubstations(s2, substation) || areDifferentSubstations(s3, substation)) {
        throw createIsolationException(substation);
    }
}

void checkRemovability(const Substation& substation, const HvdcConverterStation& station) {
    const auto& hvdcLine = substation.getNetwork().findHvdcLine(station);
    if (hvdcLine) {
        const auto& s1 = hvdcLine.get().getConverterStation1().get().getTerminal().getVoltageLevel().getNullableSubstation();
        const auto& s2 = hvdcLine.get().getConverterStation2().get().getTerminal().getVoltageLevel().getNullableSubstation();
        if (areDifferentSubstations(s1, substation) || areDifferentSubstations(s2, substation)) {
            throw createIsolationException(substation);
        }
    }
}

}  // namespace Substations

}  // namespace iidm

}  // namespace powsybl
