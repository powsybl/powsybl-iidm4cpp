/**
 * Copyright (c) 2022, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_IIDM_BRANCHOBSERVABILITYXMLSERIALIZER_HPP
#define POWSYBL_IIDM_EXTENSIONS_IIDM_BRANCHOBSERVABILITYXMLSERIALIZER_HPP

#include <powsybl/iidm/Branch.hpp>
#include <powsybl/iidm/converter/xml/AbstractExtensionXmlSerializer.hpp>

namespace powsybl {

namespace xml {

class XmlStreamWriter;

}  // namespace xml

namespace iidm {

namespace extensions {

namespace iidm {

class ObservabilityQuality;
class BranchObservabilityAdder;

class BranchObservabilityXmlSerializer : public converter::xml::AbstractExtensionXmlSerializer {
public:  // ExtensionXmlSerializer
    Extension& read(Extendable& extendable, converter::xml::NetworkXmlReaderContext& context) const override;

    void write(const Extension& extension, converter::xml::NetworkXmlWriterContext& context) const override;

public:
    BranchObservabilityXmlSerializer();

    ~BranchObservabilityXmlSerializer() noexcept override = default;

private:
    static void readQualityP(double standardDeviation, stdcxx::optional<bool> redundant, const Branch::Side& side, BranchObservabilityAdder& adder);

    static void readQualityQ(double standardDeviation, stdcxx::optional<bool> redundant, const Branch::Side& side, BranchObservabilityAdder& adder);

private:
    void writeOptionalQuality(const std::string& elementName, const Branch::Side& side, const stdcxx::CReference<ObservabilityQuality>& quality, xml::XmlStreamWriter& writer) const;
};

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_IIDM_BRANCHOBSERVABILITYXMLSERIALIZER_HPP
