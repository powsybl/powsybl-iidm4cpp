/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONVERTOR_XML_SHUNTXML_HPP
#define POWSYBL_IIDM_CONVERTOR_XML_SHUNTXML_HPP

#include "AbstractShuntCompensatorXml.hpp"

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

class ShuntXml : public AbstractShuntCompensatorXml {
public:
    static const ShuntXml& getInstance();

protected:
    // AbstractIdentifiableXml
    const char* getRootElementName() const override;

    //AbstractShuntCompensatorXml
    void assertReadCompatibilityVersion(NetworkXmlReaderContext& context) const override;
    void assertWriteCompatibilityVersion(NetworkXmlWriterContext& context) const override;

private:
    ShuntXml() = default;

    ~ShuntXml() noexcept override = default;
};

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_CONVERTOR_XML_SHUNTXML_HPP

