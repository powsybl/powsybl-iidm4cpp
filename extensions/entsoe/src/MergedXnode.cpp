/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/entsoe/MergedXnode.hpp>

#include <powsybl/iidm/Line.hpp>

namespace powsybl {

namespace iidm {

const std::string& checkNotEmpty(const std::string& value, const std::string& message);

namespace extensions {

namespace entsoe {

MergedXnode::MergedXnode(Line& line, double rdp, double xdp, double xnodeP1, double xnodeQ1, double xnodeP2, double xnodeQ2,
                         const std::string& line1Name, const std::string& line2Name, const std::string& code) :
    Extension(line),
    m_rdp(checkDividerPosition(rdp)),
    m_xdp(checkDividerPosition(xdp)),
    m_xnodeP1(checkPowerFlow(xnodeP1)),
    m_xnodeQ1(checkPowerFlow(xnodeQ1)),
    m_xnodeP2(checkPowerFlow(xnodeP2)),
    m_xnodeQ2(checkPowerFlow(xnodeQ2)),
    m_line1Name(line1Name),
    m_line2Name(line2Name),
    m_code(checkNotEmpty(code, "Xnode code is empty")) {

}

void MergedXnode::assertExtendable(const stdcxx::Reference<powsybl::iidm::Extendable>& extendable) const {
    if (extendable && !stdcxx::isInstanceOf<Line>(extendable.get())) {
        throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable.get()), stdcxx::demangle<Line>()));
    }
}

double MergedXnode::checkDividerPosition(double dp) {
    if (dp < 0.0 || dp > 1.0) {
        throw PowsyblException(stdcxx::format("Invalid divider position: %1%", dp));
    }
    return dp;
}

double MergedXnode::checkPowerFlow(double value) {
    if (std::isnan(value)) {
        throw PowsyblException(stdcxx::format("Power flow is invalid"));
    }
    return value;
}

const std::string& MergedXnode::getCode() const {
    return m_code;
}

const std::string& MergedXnode::getLine1Name() const {
    return m_line1Name;
}

const std::string& MergedXnode::getLine2Name() const {
    return m_line2Name;
}

const std::string& MergedXnode::getName() const {
    static std::string s_name = "mergedXnode";
    return s_name;
}

double MergedXnode::getRdp() const {
    return m_rdp;
}

const std::type_index& MergedXnode::getType() const {
    static std::type_index s_type = typeid(MergedXnode);
    return s_type;
}

double MergedXnode::getXdp() const {
    return m_xdp;
}

double MergedXnode::getXnodeP1() const {
    return m_xnodeP1;
}

double MergedXnode::getXnodeP2() const {
    return m_xnodeP2;
}

double MergedXnode::getXnodeQ1() const {
    return m_xnodeQ1;
}

double MergedXnode::getXnodeQ2() const {
    return m_xnodeQ2;
}

MergedXnode& MergedXnode::setCode(const std::string& xNodeCode) {
    m_code = checkNotEmpty(xNodeCode, "Xnode code is empty");
    return *this;
}

MergedXnode& MergedXnode::setLine1Name(const std::string& line1Name) {
    m_line1Name = line1Name;
    return *this;
}

MergedXnode& MergedXnode::setLine2Name(const std::string& line2Name) {
    m_line2Name = line2Name;
    return *this;
}

MergedXnode& MergedXnode::setRdp(double rdp) {
    m_rdp = checkDividerPosition(rdp);
    return *this;
}

MergedXnode& MergedXnode::setXdp(double xdp) {
    m_xdp = checkDividerPosition(xdp);
    return *this;
}

MergedXnode& MergedXnode::setXnodeP1(double xNodeP1) {
    m_xnodeP1 = checkPowerFlow(xNodeP1);
    return *this;
}

MergedXnode& MergedXnode::setXnodeQ1(double xNodeQ1) {
    m_xnodeQ1 = checkPowerFlow(xNodeQ1);
    return *this;
}

MergedXnode& MergedXnode::setXnodeP2(double xNodeP2) {
    m_xnodeP2 = checkPowerFlow(xNodeP2);
    return *this;
}

MergedXnode& MergedXnode::setXnodeQ2(double xNodeQ2) {
    m_xnodeQ2 = checkPowerFlow(xNodeQ2);
    return *this;
}

}  // namespace entsoe

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
