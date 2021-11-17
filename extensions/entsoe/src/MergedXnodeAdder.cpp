/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/entsoe/MergedXnodeAdder.hpp>

#include <powsybl/iidm/Line.hpp>
#include <powsybl/iidm/extensions/entsoe/MergedXnode.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace entsoe {

MergedXnodeAdder::MergedXnodeAdder(Extendable& extendable) :
    ExtensionAdder(extendable) {
}

std::unique_ptr<Extension> MergedXnodeAdder::createExtension(Extendable& extendable) const {
    if (stdcxx::isInstanceOf<Line>(extendable)) {
        return std::unique_ptr<MergedXnode>(new MergedXnode(dynamic_cast<Line&>(extendable), m_rdp, m_xdp, m_xnodeP1, m_xnodeQ1, m_xnodeP2, m_xnodeQ2, m_line1Name, m_line2Name, m_code));
    }
    throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable), stdcxx::demangle<Line>()));
}

MergedXnodeAdder& MergedXnodeAdder::withCode(const std::string& xNodeCode) {
    m_code = xNodeCode;
    return *this;
}

MergedXnodeAdder& MergedXnodeAdder::withLine1Name(const std::string& line1Name) {
    m_line1Name = line1Name;
    return *this;
}

MergedXnodeAdder& MergedXnodeAdder::withLine2Name(const std::string& line2Name) {
    m_line2Name = line2Name;
    return *this;
}

MergedXnodeAdder& MergedXnodeAdder::withRdp(double rdp) {
    m_rdp = rdp;
    return *this;
}

MergedXnodeAdder& MergedXnodeAdder::withXdp(double xdp) {
    m_xdp = xdp;
    return *this;
}

MergedXnodeAdder& MergedXnodeAdder::withXnodeP1(double xNodeP1) {
    m_xnodeP1 = xNodeP1;
    return *this;
}

MergedXnodeAdder& MergedXnodeAdder::withXnodeP2(double xNodeP2) {
    m_xnodeP2 = xNodeP2;
    return *this;
}

MergedXnodeAdder& MergedXnodeAdder::withXnodeQ1(double xNodeQ1) {
    m_xnodeQ1 = xNodeQ1;
    return *this;
}

MergedXnodeAdder& MergedXnodeAdder::withXnodeQ2(double xNodeQ2) {
    m_xnodeQ2 = xNodeQ2;
    return *this;
}

}  // namespace entsoe

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
