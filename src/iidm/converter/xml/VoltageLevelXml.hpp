/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONVERTER_XML_VOLTAGELEVELXML_HPP
#define POWSYBL_IIDM_CONVERTER_XML_VOLTAGELEVELXML_HPP

#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/iidm/VoltageLevelAdder.hpp>
#include <powsybl/iidm/converter/xml/AbstractIdentifiableXml.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

class VoltageLevelXml : public AbstractIdentifiableXml<VoltageLevel, VoltageLevelAdder, Container> {
public:
    static const VoltageLevelXml& getInstance();

protected:  // AbstractIdentifiableXml
    VoltageLevelAdder createAdder(Container& container) const override;

    const char* getRootElementName() const override;

    VoltageLevel& readRootElementAttributes(VoltageLevelAdder& adder, NetworkXmlReaderContext& context) const override;

    void readSubElements(VoltageLevel& voltageLevel, NetworkXmlReaderContext& context) const override;

    void writeRootElementAttributes(const VoltageLevel& voltageLevel, const Container& container, NetworkXmlWriterContext& context) const override;

    void writeSubElements(const VoltageLevel& voltageLevel, const Container& container, NetworkXmlWriterContext& context) const override;

private:
    static void writeCalculatedBus(const Bus& bus, const std::set<unsigned long>& nodes, NetworkXmlWriterContext& context);

private:
    VoltageLevelXml() = default;

    ~VoltageLevelXml() noexcept override = default;

    void readBusBreakerTopology(VoltageLevel& voltageLevel, NetworkXmlReaderContext& context) const;

    void readCalculatedBus(VoltageLevel &voltageLevel, NetworkXmlReaderContext& context) const;

    void readNodeBreakerTopology(VoltageLevel& voltageLevel, NetworkXmlReaderContext& context) const;

    void writeBatteries(const VoltageLevel& voltageLevel, NetworkXmlWriterContext& context) const;

    void writeBusBranchTopology(const VoltageLevel& voltageLevel, NetworkXmlWriterContext& context) const;

    void writeBusBreakerTopology(const VoltageLevel& voltageLevel, NetworkXmlWriterContext& context) const;

    void writeDanglingLines(const VoltageLevel& voltageLevel, NetworkXmlWriterContext& context) const;

    void writeGenerators(const VoltageLevel& voltageLevel, NetworkXmlWriterContext& context) const;

    void writeLccConverterStations(const VoltageLevel& voltageLevel, NetworkXmlWriterContext& context) const;

    void writeLoads(const VoltageLevel& voltageLevel, NetworkXmlWriterContext& context) const;

    void writeNodeBreakerTopology(const VoltageLevel& voltageLevel, NetworkXmlWriterContext& context) const;

    void writeNodeBreakerTopologyInternalConnections(const VoltageLevel& voltageLevel, NetworkXmlWriterContext& context) const;

    void writeShuntCompensators(const VoltageLevel& voltageLevel, NetworkXmlWriterContext& context) const;

    void writeStaticVarCompensators(const VoltageLevel& voltageLevel, NetworkXmlWriterContext& context) const;

    void writeVscConverterStations(const VoltageLevel& voltageLevel, NetworkXmlWriterContext& context) const;
};

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_CONVERTER_XML_VOLTAGELEVELXML_HPP

