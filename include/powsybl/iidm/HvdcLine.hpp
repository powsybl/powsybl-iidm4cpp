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

    enum class Side : unsigned char {
        ONE,
        TWO
    };

public:  // Identifiable
    const Network& getNetwork() const override;

    Network& getNetwork() override;

public:
    HvdcLine(Network& network, const std::string& id, const std::string& name, bool fictitious, double r, double nominalVoltage, double maxP,
             const ConvertersMode& convertersMode, double activePowerSetpoint, HvdcConverterStation& converterStation1, HvdcConverterStation& converterStation2);

    ~HvdcLine() noexcept override = default;

    double getActivePowerSetpoint() const;

    const ConvertersMode& getConvertersMode() const;

    stdcxx::CReference<HvdcConverterStation> getConverterStation(const Side& side) const;

    stdcxx::Reference<HvdcConverterStation> getConverterStation(const Side& side);

    stdcxx::CReference<HvdcConverterStation> getConverterStation1() const;

    stdcxx::Reference<HvdcConverterStation> getConverterStation1();

    stdcxx::CReference<HvdcConverterStation> getConverterStation2() const;

    stdcxx::Reference<HvdcConverterStation> getConverterStation2();

    double getMaxP() const;

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
    HvdcConverterStation& attach(HvdcConverterStation& converterStation);

private:
    stdcxx::Reference<HvdcConverterStation> m_converterStation1;

    stdcxx::Reference<HvdcConverterStation> m_converterStation2;

    double m_r;

    double m_nominalVoltage;

    double m_maxP;

    std::vector<ConvertersMode> m_convertersMode;

    std::vector<double> m_activePowerSetpoint;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_HVDCLINE_HPP
