/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_ACDCCONVERTERADDER_HPP
#define POWSYBL_IIDM_ACDCCONVERTERADDER_HPP

#include <powsybl/iidm/IdentifiableAdder.hpp>

#include <powsybl/iidm/AcDcConverter.hpp>
#include <powsybl/stdcxx/optional.hpp>

namespace powsybl {

namespace iidm {

class VoltageLevel;

template <typename Added, typename Adder>
class AcDcConverterAdder : public IdentifiableAdder<Added, Adder> {

public:
    ~AcDcConverterAdder() noexcept override = default;

    Adder& setNode1(unsigned long node1);
    Adder& setBus1(const std::string& bus1);
    Adder& setConnectableBus1(const std::string& connectableBus1);

    Adder& setNode2(unsigned long node2);
    Adder& setBus2(const std::string& bus2);
    Adder& setConnectableBus2(const std::string& connectableBus2);

    Adder& setDcNode1(const std::string& dcNode1);
    Adder& setDcConnected1(bool connected1);

    Adder& setDcNode2(const std::string& dcNode2);
    Adder& setDcConnected2(bool connected2);

    Adder& setIdleLoss(double idleLoss);
    Adder& setSwitchingLoss(double switchingLoss);
    Adder& setResistiveLoss(double resistiveLoss);

    Adder& setPccTerminal(const stdcxx::Reference<Terminal>& pccTerminal);

    Adder& setControlMode(const AcDcConverter::ControlMode& controlMode);

    Adder& setTargetP(double targetP);
    Adder& setTargetVdc(double targetVdc);

protected:
    AcDcConverterAdder(VoltageLevel& voltageLevel);

    std::unique_ptr<Terminal> checkAndGetTerminal1();
    std::unique_ptr<Terminal> checkAndGetTerminal2();

    bool hasTwoAcTerminals();
    void checkAcDcConverter();
    Added& checkAndAdd(std::unique_ptr<Added>&& converter);


protected: // IdentifiableAdder
    virtual const Network& getNetwork() const override;

    virtual Network& getNetwork() override;

protected:
    double m_idleLoss = 0.;
    double m_switchingLoss = 0.;
    double m_resistiveLoss = 0.;

    stdcxx::Reference<Terminal> m_pccTerminal;
    AcDcConverter::ControlMode m_controlMode = AcDcConverter::ControlMode::V_DC;
    double m_targetP = stdcxx::nan();
    double m_targetVdc = stdcxx::nan();

private:
    VoltageLevel& m_voltageLevel;

    std::string m_bus1;
    std::string m_bus2;
    std::string m_connectableBus1;
    std::string m_connectableBus2;
    stdcxx::optional<unsigned long> m_node1;
    stdcxx::optional<unsigned long> m_node2;

    std::string m_dcNode1;
    std::string m_dcNode2;
    bool m_dcConnected1 = true;
    bool m_dcConnected2 = true;

};

}  // namespace iidm

}  // namespace powsybl

#include <powsybl/iidm/AcDcConverterAdder.hxx>

#endif  // POWSYBL_IIDM_ACDCCONVERTERADDER_HPP
