/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "BoundaryLineXml.hpp"

#include <powsybl/iidm/ActivePowerLimitsAdder.hpp>
#include <powsybl/iidm/ApparentPowerLimitsAdder.hpp>
#include <powsybl/iidm/CurrentLimitsAdder.hpp>

#include "ReactiveLimitsXml.hpp"

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

const BoundaryLineXml& BoundaryLineXml::getInstance() {
    static BoundaryLineXml s_instance;
    return s_instance;
}

const char* BoundaryLineXml::getRootElementName() const  {
    return BOUNDARY_LINE;
}

void BoundaryLineXml::assertReadCompatibilityVersion(NetworkXmlReaderContext& context) const {
    IidmXmlUtil::assertMinimumVersion(getRootElementName(), "", ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_16(), context);
}
void BoundaryLineXml::assertWriteCompatibilityVersion(NetworkXmlWriterContext& context) const {
    IidmXmlUtil::assertMinimumVersion(getRootElementName(), "", ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_16(), context);
}

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

