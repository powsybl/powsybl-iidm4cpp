/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/HalfLineAdder.hpp>

#include <powsybl/iidm/TieLineAdder.hpp>
#include <powsybl/iidm/ValidationException.hpp>
#include <powsybl/stdcxx/format.hpp>

namespace powsybl {

namespace iidm {

namespace tie_line_adder {

HalfLineAdder::HalfLineAdder(TieLineAdder& parent, int num) :
    m_parent(parent),
    m_num(num) {
}

TieLineAdder& HalfLineAdder::add() {
    if (m_id.empty()) {
        throw ValidationException(m_parent.get(), stdcxx::format("id is not set for half line %1%", m_num));
    }
    if (std::isnan(m_r)) {
        throw ValidationException(m_parent.get(), stdcxx::format("r is not set for half line %1%", m_num));
    }
    if (std::isnan(m_x)) {
        throw ValidationException(m_parent.get(), stdcxx::format("x is not set for half line %1%", m_num));
    }
    if (std::isnan(m_g1)) {
        throw ValidationException(m_parent.get(), stdcxx::format("g1 is not set for half line %1%", m_num));
    }
    if (std::isnan(m_b1)) {
        throw ValidationException(m_parent.get(), stdcxx::format("b1 is not set for half line %1%", m_num));
    }
    if (std::isnan(m_g2)) {
        throw ValidationException(m_parent.get(), stdcxx::format("g2 is not set for half line %1%", m_num));
    }
    if (std::isnan(m_b2)) {
        throw ValidationException(m_parent.get(), stdcxx::format("b2 is not set for half line %1%", m_num));
    }
    if (std::isnan(m_xnodeP)) {
        throw ValidationException(m_parent.get(), stdcxx::format("xnodeP is not set for half line %1%", m_num));
    }
    if (std::isnan(m_xnodeQ)) {
        throw ValidationException(m_parent.get(), stdcxx::format("xnodeQ is not set for half line %1%", m_num));
    }
    if (m_num == 1) {
        m_parent.get().m_halfLineAdder1 = *this;
    }
    if (m_num == 2) {
        m_parent.get().m_halfLineAdder2 = *this;
    }
    return m_parent.get();
}

std::string HalfLineAdder::getMessageHeader() const {
    return stdcxx::format("TieLine.halfLine%1%", m_num);
}

HalfLineAdder& HalfLineAdder::setB1(double b1) {
    m_b1 = b1;
    return *this;
}

HalfLineAdder& HalfLineAdder::setB2(double b2) {
    m_b2 = b2;
    return *this;
}

HalfLineAdder& HalfLineAdder::setFictitious(bool fictitious) {
    m_fictitious = fictitious;
    return *this;
}

HalfLineAdder& HalfLineAdder::setG1(double g1) {
    m_g1 = g1;
    return *this;
}

HalfLineAdder& HalfLineAdder::setG2(double g2) {
    m_g2 = g2;
    return *this;
}

HalfLineAdder& HalfLineAdder::setId(const std::string& id) {
    m_id = id;
    return *this;
}

HalfLineAdder& HalfLineAdder::setName(const std::string& name) {
    m_name = name;
    return *this;
}

HalfLineAdder& HalfLineAdder::setR(double r) {
    m_r = r;
    return *this;
}

HalfLineAdder& HalfLineAdder::setX(double x) {
    m_x = x;
    return *this;
}

HalfLineAdder& HalfLineAdder::setXnodeP(double xnodeP) {
    m_xnodeP = xnodeP;
    return *this;
}

HalfLineAdder& HalfLineAdder::setXnodeQ(double xnodeQ) {
    m_xnodeQ = xnodeQ;
    return *this;
}

}  // namespace tie_line_adder

}  // namespace iidm

}  // namespace powsybl
