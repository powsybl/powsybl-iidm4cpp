/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_HVDCLINE_HPP
#define POWSYBL_IIDM_HVDCLINE_HPP

#include <string>
#include <vector>

#include <powsybl/iidm/Identifiable.hpp>
#include <powsybl/iidm/MultiVariantObject.hpp>
#include <powsybl/stdcxx/reference_wrapper.hpp>

namespace powsybl {

namespace iidm {

class HvdcConverterStation;
class Network;

class HvdcLine : public Identifiable, public MultiVariantObject {
public:
    enum class ConvertersMode : unsigned char {
        SIDE_1_RECTIFIER_SIDE_2_INVERTER,
        SIDE_1_INVERTER_SIDE_2_RECTIFIER
    };

public:
    HvdcLine(Network& network, const std::string& id, const std::string& name, double r, double nominalVoltage, double maxP,
             const ConvertersMode& convertersMode, double activePowerSetpoint, HvdcConverterStation& converterStation1, HvdcConverterStation& converterStation2);

    ~HvdcLine() noexcept override = default;

    double getActivePowerSetpoint() const;

    const ConvertersMode& getConvertersMode() const;

    const stdcxx::Reference<HvdcConverterStation>& getConverterStation1() const;

    stdcxx::Reference<HvdcConverterStation>& getConverterStation1();

    const stdcxx::Reference<HvdcConverterStation>& getConverterStation2() const;

    stdcxx::Reference<HvdcConverterStation>& getConverterStation2();

    double getMaxP() const;

    const Network& getNetwork() const;

    Network& getNetwork();

    double getNominalVoltage() const;

    double getR() const;

    void remove();

    HvdcLine& setActivePowerSetpoint(double activePowerSetpoint);

    HvdcLine& setConvertersMode(const ConvertersMode& mode);

    HvdcLine& setMaxP(double maxP);

    HvdcLine& setNominalVoltage(double nominalVoltage);

    HvdcLine& setR(double r);

protected: // MultiVariantObject
    void allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) override;

    void deleteVariantArrayElement(unsigned long index) override;

    void extendVariantArraySize(unsigned long initVariantArraySize, unsigned long number, unsigned long sourceIndex) override;

    void reduceVariantArraySize(unsigned long number) override;

private: // Identifiable
    const std::string& getTypeDescription() const override;

private:
    stdcxx::Reference<Network> m_network;

    stdcxx::Reference<HvdcConverterStation> m_converterStation1;

    stdcxx::Reference<HvdcConverterStation> m_converterStation2;

    double m_r;

    double m_nominalVoltage;

    double m_maxP;

    std::vector<ConvertersMode> m_convertersMode;

    std::vector<double> m_activePowerSetpoint;
};

// FIXME(sebalaig) should be updated while fixing issue #43 (https://devin-source.rte-france.com/powsybl/powsybl-iidm/issues/43)
HvdcLine::ConvertersMode getConvertersMode(const std::string& convertersModeName);

std::string getConvertersModeName(const HvdcLine::ConvertersMode& convertersMode);

std::ostream& operator<<(std::ostream& stream, const HvdcLine::ConvertersMode& mode);

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_HVDCLINE_HPP
