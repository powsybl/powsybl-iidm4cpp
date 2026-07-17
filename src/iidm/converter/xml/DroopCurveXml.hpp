/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONVERTER_XML_DROOPCURVEXML_HPP
#define POWSYBL_IIDM_CONVERTER_XML_DROOPCURVEXML_HPP

#include <powsybl/iidm/AcDcConverter.hpp>
#include <powsybl/iidm/DroopCurve.hpp>
#include <powsybl/iidm/DroopCurveAdder.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

class NetworkXmlReaderContext;
class NetworkXmlWriterContext;

class DroopCurveXml {
public:
    static const DroopCurveXml& getInstance();

    void read(AcDcConverter& converter, const NetworkXmlReaderContext& context) const;

    void write(const AcDcConverter& converter, NetworkXmlWriterContext& context) const;

private:
    DroopCurveXml() = default;

    ~DroopCurveXml() noexcept = default;

    void read(DroopCurveAdder& curveAdder, const NetworkXmlReaderContext& context) const;

    void write(const DroopCurve& droopCurve, NetworkXmlWriterContext& context) const;
};

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_CONVERTER_XML_DROOPCURVEXML_HPP

