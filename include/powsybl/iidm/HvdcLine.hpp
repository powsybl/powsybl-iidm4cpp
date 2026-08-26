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
#include <powsybl/iidm/NetworkRef.hpp>
#include <powsybl/iidm/TwoSides.hpp>
#include <powsybl/stdcxx/Predicate.hpp>
#include <powsybl/stdcxx/reference.hpp>


namespace powsybl {

namespace iidm {

class HvdcConverterStation;
class Network;
class Switch;
class Terminal;

class HvdcLine : public Identifiable {
public:
    enum class ConvertersMode : unsigned char {
        SIDE_1_RECTIFIER_SIDE_2_INVERTER,
        SIDE_1_INVERTER_SIDE_2_RECTIFIER
    };

public:  // Identifiable
    const Network& getNetwork() const override;

    Network& getNetwork() override;

    const Network& getParentNetwork() const override;

    Network& getParentNetwork() override;

    const IdentifiableType& getType() const override;

public:
    HvdcLine(Network& network, const std::string& id, const std::string& name, bool fictitious, double r, double nominalV, double maxP,
             const ConvertersMode& convertersMode, double activePowerSetpoint, HvdcConverterStation& converterStation1, HvdcConverterStation& converterStation2);

    ~HvdcLine() noexcept override = default;

    double getActivePowerSetpoint() const;

    const ConvertersMode& getConvertersMode() const;

    stdcxx::CReference<HvdcConverterStation> getConverterStation(const TwoSides& side) const;

    stdcxx::Reference<HvdcConverterStation> getConverterStation(const TwoSides& side);

    stdcxx::CReference<HvdcConverterStation> getConverterStation1() const;

    stdcxx::Reference<HvdcConverterStation> getConverterStation1();

    stdcxx::CReference<HvdcConverterStation> getConverterStation2() const;

    stdcxx::Reference<HvdcConverterStation> getConverterStation2();

    double getMaxP() const;

    double getNominalV() const;

    double getR() const;

    void remove();

    HvdcLine& setActivePowerSetpoint(double activePowerSetpoint);

    HvdcLine& setConvertersMode(const ConvertersMode& mode);

    HvdcLine& setMaxP(double maxP);

    HvdcLine& setNominalV(double nominalV);

    HvdcLine& setR(double r);

    /**
     * Try to connect the converter stations on all sides, by operating on non-fictional breakers
     */
    bool connectConverterStations();
    /**
     * Try to connect the converter stations, by operating on switches that respect the given predicate.
     */
    bool connectConverterStations(const stdcxx::Predicate<Switch>& isTypeSwitchToOperate);
    /**
     * Try to connect the converter stations on the given side, by operating on switches that respect the given predicate.
     */
    bool connectConverterStations(const stdcxx::Predicate<Switch>& isTypeSwitchToOperate, const stdcxx::optional<TwoSides>& side);

    /**
     * Try to disconnect the converter stations on all sides, by operating on closed non-fictional breakers
     */
    bool disconnectConverterStations();
    /**
     * Try to disconnect the converter stations on all sides, by operating on switches that respect the given predicate.
     */
    bool disconnectConverterStations(const stdcxx::Predicate<Switch>& isSwitchOpenable);
    /**
     * Try to disconnect the converter stations on the given side, by operating on switches that respect the given predicate.
     */
    bool disconnectConverterStations(const stdcxx::Predicate<Switch>& isSwitchOpenable, const stdcxx::optional<TwoSides>& side);

protected: // MultiVariantObject
    void allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) override;

    void extendVariantArraySize(unsigned long initVariantArraySize, unsigned long number, unsigned long sourceIndex) override;

    void reduceVariantArraySize(unsigned long number) override;

private: // Identifiable
    const std::string& getTypeDescription() const override;

private:
    HvdcConverterStation& attach(HvdcConverterStation& converterStation);

    std::vector<std::reference_wrapper<Terminal>> getTerminalsOfConverterStations(const stdcxx::optional<TwoSides>& side);

private:
    void setNetworkRef(Network& network);
    friend class NetworkIndex;

private:
    NetworkRef m_network;

    stdcxx::Reference<HvdcConverterStation> m_converterStation1;

    stdcxx::Reference<HvdcConverterStation> m_converterStation2;

    double m_r;

    double m_nominalV;

    double m_maxP;

    std::vector<ConvertersMode> m_convertersMode;

    std::vector<double> m_activePowerSetpoint;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_HVDCLINE_HPP
