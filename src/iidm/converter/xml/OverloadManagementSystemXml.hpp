/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONVERTOR_XML_OVERLOADMANAGEMENTSYSTEMXML_HPP
#define POWSYBL_IIDM_CONVERTOR_XML_OVERLOADMANAGEMENTSYSTEMXML_HPP

#include <powsybl/iidm/OverloadManagementSystem.hpp>
#include <powsybl/iidm/OverloadManagementSystemAdder.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/converter/xml/AbstractComplexIdentifiableXml.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

class OverloadManagementSystemXml : public AbstractComplexIdentifiableXml<OverloadManagementSystem, OverloadManagementSystemAdder, Substation> {
public:
    static const OverloadManagementSystemXml& getInstance();

    void skip(NetworkXmlReaderContext& context) const;

protected:  
    // AbstractIdentifiableXml
    OverloadManagementSystemAdder createAdder(Substation& substation) const override;

    // AbstractIdentifiableXml
    const char* getRootElementName() const override;

    // AbstractComplexIdentifiableXml
    void readRootElementAttributes(OverloadManagementSystemAdder& adder, std::vector<std::function<void(Identifiable&)>>& toApply, NetworkXmlReaderContext& context) const override;
    void skipRootElementAttributes(NetworkXmlReaderContext& context) const;
    void getRootElementAttributes(NetworkXmlReaderContext& context, bool& enabled, std::string& monitoredId, ThreeSides& side) const;

    // AbstractComplexIdentifiableXml
    void readSubElements(const std::string& id, OverloadManagementSystemAdder& adder, std::vector<std::function<void(Identifiable&)>>& toApply, NetworkXmlReaderContext& context) const override;
    void skipSubElements(const std::string& id, NetworkXmlReaderContext& context) const;

    // AbstractComplexIdentifiableXml
    stdcxx::optional<XmlReaderEndTask::Step> postponeElementCreation() const override;
    // AbstractComplexIdentifiableXml
    void readAndPostponeCreation(Substation& substation, NetworkXmlReaderContext& context) const override;

    // AbstractIdentifiableXml
    void writeRootElementAttributes(const OverloadManagementSystem& oms, const Substation& substation, NetworkXmlWriterContext& context) const override;

    void writeSubElements(const OverloadManagementSystem& oms, const Substation& substation, NetworkXmlWriterContext& context) const override;

private:
    void writeTripping(const OverloadManagementSystem::Tripping& tripping, NetworkXmlWriterContext& context) const;
    void writeTrippingCommonAttributes(const OverloadManagementSystem::Tripping& tripping, NetworkXmlWriterContext& context) const;

    void readTrippingCommonAttributes(overload_management_system::TrippingAdder& trippingAdder, NetworkXmlReaderContext& context) const;
    void skipTrippingCommonAttributes(NetworkXmlReaderContext& context) const;
    void getTrippingCommonAttributes(NetworkXmlReaderContext& context, std::string& key, std::string& name, double& currentLimit, bool& openAction) const;
    void readBranchTripping(OverloadManagementSystemAdder& adder, NetworkXmlReaderContext& context) const;
    void skipBranchTripping(NetworkXmlReaderContext& context) const;
    void getBranchTrippingAttributes(NetworkXmlReaderContext& context, std::string& branchId, TwoSides& side) const;
    void readSwitchTripping(OverloadManagementSystemAdder& adder, NetworkXmlReaderContext& context) const;
    void skipSwitchTripping(NetworkXmlReaderContext& context) const;
    void getSwitchTrippingAttributes(NetworkXmlReaderContext& context, std::string& switchId) const;
    void readThreeWindingsTransformerTripping(OverloadManagementSystemAdder& adder, NetworkXmlReaderContext& context) const;
    void skipThreeWindingsTransformerTripping(NetworkXmlReaderContext& context) const;
    void getThreeWindingsTransformerTrippingAttributes(NetworkXmlReaderContext& context, std::string& twtId, ThreeSides& side) const;

private:
    OverloadManagementSystemXml() = default;

    ~OverloadManagementSystemXml() noexcept override = default;
};

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_CONVERTOR_XML_OVERLOADMANAGEMENTSYSTEMXML_HPP

