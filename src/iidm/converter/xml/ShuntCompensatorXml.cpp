/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ShuntCompensatorXml.hpp"

#include <powsybl/iidm/converter/Constants.hpp>

#include <powsybl/stdcxx/format.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {


const ShuntCompensatorXml& ShuntCompensatorXml::getInstance() {
    static ShuntCompensatorXml s_instance;
    return s_instance;
}

const char* ShuntCompensatorXml::getRootElementName() const {
    return SHUNT_COMPENSATOR;
}

void ShuntCompensatorXml::assertReadCompatibilityVersion(NetworkXmlReaderContext& context) const {
    IidmXmlUtil::assertMinimumVersion(getRootElementName(), "", ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_16(), context);
}

void ShuntCompensatorXml::assertWriteCompatibilityVersion(NetworkXmlWriterContext& context) const {
    IidmXmlUtil::assertMinimumVersion(getRootElementName(), "", ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_16(), context);
}


}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

