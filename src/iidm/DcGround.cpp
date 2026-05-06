/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/DcGround.hpp>

#include <powsybl/iidm/ValidationUtils.hpp>

namespace powsybl {

namespace iidm {

DcGround::DcGround(const std::string& id, const std::string& name, bool fictitious, double r) :
    Identifiable(id, name, fictitious),
    m_r(checkDoubleParamPositive(*this, r, "r")) {
}

const IdentifiableType& DcGround::getType() const {
    static IdentifiableType s_type = IdentifiableType::DC_GROUND;
    return s_type;
}

const std::string& DcGround::getTypeDescription() const {
    static std::string s_typeDescription = "DC Ground";
    return s_typeDescription;
}

const DcTerminal& DcGround::getDcTerminal() const {
    return DcConnectable::getDcTerminal(0);
}
DcTerminal& DcGround::getDcTerminal() {
    return DcConnectable::getDcTerminal(0);
}

double DcGround::getR() const {
    return m_r;
}
DcGround& DcGround::setR(double r) {
    checkDoubleParamPositive(*this, r, "r");
    m_r = r;
    return *this;
}


}  // namespace iidm

}  // namespace powsybl
