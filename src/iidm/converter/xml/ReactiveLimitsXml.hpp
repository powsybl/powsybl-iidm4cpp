/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONVERTER_XML_REACTIVELIMITSXML_HPP
#define POWSYBL_IIDM_CONVERTER_XML_REACTIVELIMITSXML_HPP

namespace powsybl {

namespace iidm {

class MinMaxReactiveLimits;
class ReactiveCapabilityCurve;
class ReactiveLimitsHolder;

namespace converter {

namespace xml {

class NetworkXmlReaderContext;
class NetworkXmlWriterContext;

class ReactiveLimitsXml {
public:
    static const ReactiveLimitsXml& getInstance();

public:
    void read(ReactiveLimitsHolder& holder, const NetworkXmlReaderContext& context) const;

    void write(const ReactiveLimitsHolder& holder, NetworkXmlWriterContext& context) const;

private:
    ReactiveLimitsXml() = default;

    ~ReactiveLimitsXml() noexcept = default;

    void write(const MinMaxReactiveLimits& limits, NetworkXmlWriterContext& context) const;

    void write(const ReactiveCapabilityCurve& curve, NetworkXmlWriterContext& context) const;
};

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_CONVERTER_XML_REACTIVELIMITSXML_HPP

