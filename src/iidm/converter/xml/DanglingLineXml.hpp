/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONVERTER_XML_DANGLINGLINEXML_HPP
#define POWSYBL_IIDM_CONVERTER_XML_DANGLINGLINEXML_HPP

#include "AbstractBoundaryLineXml.hpp"

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

class DanglingLineXml : public AbstractBoundaryLineXml {
public:
    static const DanglingLineXml& getInstance();

protected:  // AbstractIdentifiableXml
    const char* getRootElementName() const override;

    void assertReadCompatibilityVersion(NetworkXmlReaderContext& context) const override;
    void assertWriteCompatibilityVersion(NetworkXmlWriterContext& context) const override;

private:
    DanglingLineXml() = default;

    ~DanglingLineXml() noexcept override = default;
};

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_CONVERTER_XML_DANGLINGLINEXML_HPP

