/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ShuntXml.hpp"

#include <powsybl/iidm/converter/Constants.hpp>

#include <powsybl/stdcxx/format.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {


const ShuntXml& ShuntXml::getInstance() {
    static ShuntXml s_instance;
    return s_instance;
}

const char* ShuntXml::getRootElementName() const {
    return SHUNT;
}

void ShuntXml::assertReadCompatibilityVersion(NetworkXmlReaderContext& context) const {
    IidmXmlUtil::assertMaximumVersion(getRootElementName(), "", ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_15(), context);
}

void ShuntXml::assertWriteCompatibilityVersion(NetworkXmlWriterContext& context) const {
    IidmXmlUtil::assertMaximumVersion(getRootElementName(), "", ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_15(), context);
}


}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

