/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/cvg/HvdcOperatorActivePowerRange.hpp>

#include <cmath>

#include <powsybl/iidm/HvdcConverterStation.hpp>
#include <powsybl/iidm/HvdcLine.hpp>
#include <powsybl/logging/MessageFormat.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace cvg {

HvdcOperatorActivePowerRange::HvdcOperatorActivePowerRange(HvdcLine& hvdcLine, double oprFromCS1toCS2, double oprFromCS2toCS1) :
    Extension(hvdcLine),
    m_oprFromCS1toCS2(checkOPR(oprFromCS1toCS2, hvdcLine.getConverterStation1(), hvdcLine.getConverterStation2())),
    m_oprFromCS2toCS1(checkOPR(oprFromCS2toCS1, hvdcLine.getConverterStation2(), hvdcLine.getConverterStation1())) {

}

void HvdcOperatorActivePowerRange::assertExtendable(const stdcxx::Reference<Extendable>& extendable) const {
    if (extendable && !stdcxx::isInstanceOf<HvdcLine>(extendable.get())) {
        throw AssertionError(logging::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable.get()), stdcxx::demangle<HvdcLine>()));
    }
}


double HvdcOperatorActivePowerRange::checkOPR(double opr, const HvdcConverterStation& from, const HvdcConverterStation& to) {
    if ((!std::isnan(opr)) && (opr < 0)) {
        throw PowsyblException(logging::format("OPR from %1% to %2% must be greater than 0.", from.getId(), to.getId()));
    }
    return opr;
}

const std::string& HvdcOperatorActivePowerRange::getName() const {
    static std::string s_name = "hvdcOperatorActivePowerRange";
    return s_name;
}

double HvdcOperatorActivePowerRange::getOprFromCS1toCS2() const {
    return m_oprFromCS1toCS2;
}

double HvdcOperatorActivePowerRange::getOprFromCS2toCS1() const {
    return m_oprFromCS2toCS1;
}

const std::type_index& HvdcOperatorActivePowerRange::getType() const {
    static std::type_index s_type = typeid(HvdcOperatorActivePowerRange);
    return s_type;
}

HvdcOperatorActivePowerRange& HvdcOperatorActivePowerRange::setOprFromCS1toCS2(double oprFromCS1toCS2) {
    const HvdcLine& hvdcLine = getExtendable<HvdcLine>().get();
    m_oprFromCS1toCS2 = checkOPR(oprFromCS1toCS2, hvdcLine.getConverterStation1(), hvdcLine.getConverterStation2());
    return *this;
}

HvdcOperatorActivePowerRange& HvdcOperatorActivePowerRange::setOprFromCS2toCS1(double oprFromCS2toCS1) {
    const HvdcLine& hvdcLine = getExtendable<HvdcLine>().get();
    m_oprFromCS2toCS1 = checkOPR(oprFromCS2toCS1, hvdcLine.getConverterStation2(), hvdcLine.getConverterStation1());
    return *this;
}

}  // namespace cvg

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

