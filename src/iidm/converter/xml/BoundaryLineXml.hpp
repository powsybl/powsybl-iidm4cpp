/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONVERTER_XML_BOUNDARYLINEXML_HPP
#define POWSYBL_IIDM_CONVERTER_XML_BOUNDARYLINEXML_HPP

#include "AbstractBoundaryLineXml.hpp"

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

class BoundaryLineXml : public AbstractBoundaryLineXml {
public:
    static const BoundaryLineXml& getInstance();

protected:  // AbstractIdentifiableXml
    const char* getRootElementName() const override;

    void assertReadCompatibilityVersion(NetworkXmlReaderContext& context) const;
    void assertWriteCompatibilityVersion(NetworkXmlWriterContext& context) const;

private:
    BoundaryLineXml() = default;

    ~BoundaryLineXml() noexcept override = default;
};

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_CONVERTER_XML_BOUNDARYLINEXML_HPP

