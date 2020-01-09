/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/cvg/LoadDetail.hpp>

#include <cmath>

#include <powsybl/PowsyblException.hpp>
#include <powsybl/iidm/Load.hpp>
#include <powsybl/logging/MessageFormat.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace cvg {

LoadDetail::LoadDetail(Load& load, double fixedActivePower, double fixedReactivePower, double variableActivePower, double variableReactivePower) :
    Extension(load),
    m_fixedActivePower(checkPower(fixedActivePower, logging::format("Invalid fixedActivePower"))),
    m_fixedReactivePower(checkPower(fixedReactivePower, logging::format("Invalid fixedReactivePower"))),
    m_variableActivePower(checkPower(variableActivePower, logging::format("Invalid variableActivePower"))),
    m_variableReactivePower(checkPower(variableReactivePower, logging::format("Invalid variableReactivePower"))) {
}

void LoadDetail::assertExtendable(const stdcxx::Reference<Extendable>& extendable) const {
    if (extendable && !stdcxx::isInstanceOf<Load>(extendable.get())) {
        throw AssertionError(logging::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable.get()), stdcxx::demangle<Load>()));
    }
}

double LoadDetail::checkPower(double value, const std::string& message) {
    if (std::isnan(value)) {
        throw PowsyblException(message);
    }
    return value;
}

double LoadDetail::getFixedActivePower() const {
    return m_fixedActivePower;
}

double LoadDetail::getFixedReactivePower() const {
    return m_fixedReactivePower;
}

const std::string& LoadDetail::getName() const {
    static std::string s_name = "detail";
    return s_name;
}

const std::type_index& LoadDetail::getType() const {
    static std::type_index s_type = typeid(LoadDetail);
    return s_type;
}

double LoadDetail::getVariableActivePower() const {
    return m_variableActivePower;
}

double LoadDetail::getVariableReactivePower() const {
    return m_variableReactivePower;
}

LoadDetail& LoadDetail::setFixedActivePower(double fixedActivePower) {
    m_fixedActivePower = checkPower(fixedActivePower, logging::format("Invalid fixedActivePower"));
    return *this;
}

LoadDetail& LoadDetail::setFixedReactivePower(double fixedReactivePower) {
    m_fixedReactivePower = checkPower(fixedReactivePower, logging::format("Invalid fixedReactivePower"));
    return *this;
}

LoadDetail& LoadDetail::setVariableActivePower(double variableActivePower) {
    m_variableActivePower = checkPower(variableActivePower, logging::format("Invalid variableActivePower"));
    return *this;
}

LoadDetail& LoadDetail::setVariableReactivePower(double variableReactivePower) {
    m_variableReactivePower = checkPower(variableReactivePower, logging::format("Invalid variableReactivePower"));
    return *this;
}

}  // namespace cvg

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

