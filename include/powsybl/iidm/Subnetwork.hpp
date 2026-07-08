/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_SUBNETWORK_HPP
#define POWSYBL_IIDM_SUBNETWORK_HPP

#include <powsybl/iidm/Network.hpp>

namespace powsybl {

namespace iidm {

class Subnetwork : public Network {

public:
    Subnetwork(Network& parentNetwork, const std::string& id, const std::string& sourceFormat);

    Subnetwork(const Subnetwork&) = delete;

    Subnetwork(Subnetwork&& network) noexcept;

    ~Subnetwork() noexcept override = default;

    Subnetwork& operator=(const Subnetwork&) = delete;

    Subnetwork& operator=(Subnetwork&&) noexcept = delete;

public:  // Identifiable
    const Network& getNetwork() const override;
    Network& getNetwork() override;

public:  // VariantManagerHolder
    unsigned long getVariantIndex() const override;
    const VariantManager& getVariantManager() const override;
    VariantManager& getVariantManager() override;

public:
    stdcxx::CReference<HvdcLine> findHvdcLine(const HvdcConverterStation& station) const override;
    stdcxx::Reference<HvdcLine> findHvdcLine(const HvdcConverterStation& station) override;

    unsigned long getAreaCount() const override;
    stdcxx::const_range<Area> getAreas() const override;
    stdcxx::range<Area> getAreas() override;
    const Area& getArea(const std::string& id) const override;
    Area& getArea(const std::string& id) override;

    const Battery& getBattery(const std::string& id) const override;
    Battery& getBattery(const std::string& id) override;
    unsigned long getBatteryCount() const override;
    stdcxx::const_range<Battery> getBatteries() const override;
    stdcxx::range<Battery> getBatteries() override;

    const Branch& getBranch(const std::string& id) const override;
    Branch& getBranch(const std::string& id) override;
    unsigned long getBranchCount() const override;
    stdcxx::const_range<Branch> getBranches() const override;
    stdcxx::range<Branch> getBranches() override;

    const BusbarSection& getBusbarSection(const std::string& id) const override;
    BusbarSection& getBusbarSection(const std::string& id) override;
    unsigned long getBusbarSectionCount() const override;
    stdcxx::const_range<BusbarSection> getBusbarSections() const override;
    stdcxx::range<BusbarSection> getBusbarSections() override;

    const network::BusBreakerView& getBusBreakerView() const override;
    network::BusBreakerView& getBusBreakerView() override;

    const network::BusView& getBusView() const override;
    network::BusView& getBusView() override;

    const DanglingLine& getDanglingLine(const std::string& id) const override;
    DanglingLine& getDanglingLine(const std::string& id) override;
    stdcxx::const_range<DanglingLine> getDanglingLines(const DanglingLineFilter& filter) const override;
    stdcxx::range<DanglingLine> getDanglingLines(const DanglingLineFilter& filter) override;
    unsigned long getDanglingLineCount() const override;
    stdcxx::const_range<DanglingLine> getDanglingLines() const override;
    stdcxx::range<DanglingLine> getDanglingLines() override;

    stdcxx::CReference<DcBus> getDcBus(const std::string& id) const override;
    stdcxx::Reference<DcBus> getDcBus(const std::string& id) override;
    unsigned long getDcBusCount() const override;
    stdcxx::const_range<DcBus> getDcBuses() const override;
    stdcxx::range<DcBus> getDcBuses() override;

    stdcxx::const_range<Component> getDcComponents() const override;
    stdcxx::range<Component> getDcComponents() override;

    const DcLine& getDcLine(const std::string& id) const override;
    DcLine& getDcLine(const std::string& id) override;
    unsigned long getDcLineCount() const override;
    stdcxx::const_range<DcLine> getDcLines() const override;
    stdcxx::range<DcLine> getDcLines() override;

    const DcGround& getDcGround(const std::string& id) const override;
    DcGround& getDcGround(const std::string& id) override;
    unsigned long getDcGroundCount() const override;
    stdcxx::const_range<DcGround> getDcGrounds() const override;
    stdcxx::range<DcGround> getDcGrounds() override;

    const DcNode& getDcNode(const std::string& id) const override;
    DcNode& getDcNode(const std::string& id) override;
    unsigned long getDcNodeCount() const override;
    stdcxx::const_range<DcNode> getDcNodes() const override;
    stdcxx::range<DcNode> getDcNodes() override;

    const DcSwitch& getDcSwitch(const std::string& id) const override;
    DcSwitch& getDcSwitch(const std::string& id) override;
    unsigned long getDcSwitchCount() const override;
    stdcxx::const_range<DcSwitch> getDcSwitches() const override;
    stdcxx::range<DcSwitch> getDcSwitches() override;

    const Generator& getGenerator(const std::string& id) const override;
    Generator& getGenerator(const std::string& id) override;
    unsigned long getGeneratorCount() const  override;
    stdcxx::const_range<Generator> getGenerators() const override;
    stdcxx::range<Generator> getGenerators() override;

    const Ground& getGround(const std::string& id) const override;
    Ground& getGround(const std::string& id) override;
    unsigned long getGroundCount() const override;
    stdcxx::const_range<Ground> getGrounds() const override;
    stdcxx::range<Ground> getGrounds() override;
    
    const HvdcConverterStation& getHvdcConverterStation(const std::string& id) const override;
    HvdcConverterStation& getHvdcConverterStation(const std::string& id) override;
    unsigned long getHvdcConverterStationCount() const override;
    stdcxx::const_range<HvdcConverterStation> getHvdcConverterStations() const override;
    stdcxx::range<HvdcConverterStation> getHvdcConverterStations() override;

    const HvdcLine& getHvdcLine(const std::string& id) const override;
    HvdcLine& getHvdcLine(const std::string& id) override;
    const HvdcLine& getHvdcLine(const HvdcConverterStation& station) const override;
    HvdcLine& getHvdcLine(const HvdcConverterStation& station) override;
    unsigned long getHvdcLineCount() const override;
    stdcxx::const_range<HvdcLine> getHvdcLines() const override;
    stdcxx::range<HvdcLine> getHvdcLines() override;

    const Identifiable& getIdentifiable(const std::string& id) const override;
    Identifiable& getIdentifiable(const std::string& id) override;
    stdcxx::const_range<Identifiable> getIdentifiables() const override;
    stdcxx::range<Identifiable> getIdentifiables() override;

    const LccConverterStation& getLccConverterStation(const std::string& id) const override;
    LccConverterStation& getLccConverterStation(const std::string& id) override;
    unsigned long getLccConverterStationCount() const override;
    stdcxx::const_range<LccConverterStation> getLccConverterStations() const override;
    stdcxx::range<LccConverterStation> getLccConverterStations() override;

    const Line& getLine(const std::string& id) const override;
    Line& getLine(const std::string& id) override;
    unsigned long getLineCount() const override;
    stdcxx::const_range<Line> getLines() const override;
    stdcxx::range<Line> getLines() override;

    const LineCommutatedConverter& getLineCommutatedConverter(const std::string& id) const override;
    LineCommutatedConverter& getLineCommutatedConverter(const std::string& id) override;
    unsigned long getLineCommutatedConverterCount() const override;
    stdcxx::const_range<LineCommutatedConverter> getLineCommutatedConverters() const override;
    stdcxx::range<LineCommutatedConverter> getLineCommutatedConverters() override;

    const TieLine& getTieLine(const std::string& id) const override;
    TieLine& getTieLine(const std::string& id) override;
    unsigned long getTieLineCount() const override;
    stdcxx::const_range<TieLine> getTieLines() const override;
    stdcxx::range<TieLine> getTieLines() override;

    const Load& getLoad(const std::string& id) const override;
    Load& getLoad(const std::string& id) override;
    unsigned long getLoadCount() const override;
    stdcxx::const_range<Load> getLoads() const override;
    stdcxx::range<Load> getLoads() override;

    const OverloadManagementSystem& getOverloadManagementSystem(const std::string& id) const override;
    OverloadManagementSystem& getOverloadManagementSystem(const std::string& id) override;
    unsigned long getOverloadManagementSystemCount() const override;
    stdcxx::const_range<OverloadManagementSystem> getOverloadManagementSystems() const override;
    stdcxx::range<OverloadManagementSystem> getOverloadManagementSystems() override;

    const ShuntCompensator& getShuntCompensator(const std::string& id) const override;
    ShuntCompensator& getShuntCompensator(const std::string& id) override;
    unsigned long getShuntCompensatorCount() const override;
    stdcxx::const_range<ShuntCompensator> getShuntCompensators() const override;
    stdcxx::range<ShuntCompensator> getShuntCompensators() override;

    const StaticVarCompensator& getStaticVarCompensator(const std::string& id) const override;
    StaticVarCompensator& getStaticVarCompensator(const std::string& id) override;
    unsigned long getStaticVarCompensatorCount() const override;
    stdcxx::const_range<StaticVarCompensator> getStaticVarCompensators() const override;
    stdcxx::range<StaticVarCompensator> getStaticVarCompensators() override;

    stdcxx::CReference<Network> getSubNetwork(const std::string& id) const override;
    stdcxx::Reference<Network> getSubNetwork(const std::string& id) override;
    unsigned long getSubNetworksCount() const override;
    stdcxx::const_range<Network> getSubNetworks() const override;
    stdcxx::range<Network> getSubNetworks() override;

    const Substation& getSubstation(const std::string& id) const override;
    Substation& getSubstation(const std::string& id) override;
    unsigned long getSubstationCount() const override;
    stdcxx::const_range<Substation> getSubstations() const override;
    stdcxx::range<Substation> getSubstations() override;

    const Switch& getSwitch(const std::string& id) const override;
    Switch& getSwitch(const std::string& id) override;
    unsigned long getSwitchCount() const override;
    stdcxx::const_range<Switch> getSwitches() const override;
    stdcxx::range<Switch> getSwitches() override;

    const ThreeWindingsTransformer& getThreeWindingsTransformer(const std::string& id) const override;
    ThreeWindingsTransformer& getThreeWindingsTransformer(const std::string& id) override;
    unsigned long getThreeWindingsTransformerCount() const override;
    stdcxx::const_range<ThreeWindingsTransformer> getThreeWindingsTransformers() const override;
    stdcxx::range<ThreeWindingsTransformer> getThreeWindingsTransformers() override;

    const TwoWindingsTransformer& getTwoWindingsTransformer(const std::string& id) const override;
    TwoWindingsTransformer& getTwoWindingsTransformer(const std::string& id) override;
    unsigned long getTwoWindingsTransformerCount() const override;
    stdcxx::const_range<TwoWindingsTransformer> getTwoWindingsTransformers() const override;
    stdcxx::range<TwoWindingsTransformer> getTwoWindingsTransformers() override;

    const VoltageAngleLimit& getVoltageAngleLimit(const std::string& id) const override;
    VoltageAngleLimit& getVoltageAngleLimit(const std::string& id) override;
    stdcxx::const_range<VoltageAngleLimit> getVoltageAngleLimits() const override;
    stdcxx::range<VoltageAngleLimit> getVoltageAngleLimits() override;
    std::map<std::string, VoltageAngleLimit>& getVoltageAngleLimitsIndex() override;

    const VoltageLevel& getVoltageLevel(const std::string& id) const override;
    VoltageLevel& getVoltageLevel(const std::string& id) override;
    unsigned long getVoltageLevelCount() const override;
    stdcxx::const_range<VoltageLevel> getVoltageLevels() const override;
    stdcxx::range<VoltageLevel> getVoltageLevels() override;

    const VoltageSourceConverter& getVoltageSourceConverter(const std::string& id) const override;
    VoltageSourceConverter& getVoltageSourceConverter(const std::string& id) override;
    unsigned long getVoltageSourceConverterCount() const override;
    stdcxx::const_range<VoltageSourceConverter> getVoltageSourceConverters() const override;
    stdcxx::range<VoltageSourceConverter> getVoltageSourceConverters() override;

    const VscConverterStation& getVscConverterStation(const std::string& id) const override;
    VscConverterStation& getVscConverterStation(const std::string& id) override;
    unsigned long getVscConverterStationCount() const override;
    stdcxx::const_range<VscConverterStation> getVscConverterStations() const override;
    stdcxx::range<VscConverterStation> getVscConverterStations() override;

    AreaAdder newArea() override;
    DcLineAdder newDcLine() override;
    DcGroundAdder newDcGround() override;
    DcNodeAdder newDcNode() override;
    DcSwitchAdder newDcSwitch() override;
    HvdcLineAdder newHvdcLine() override;
    HvdcLineAdder newHvdcLine(const std::string& subNetworkId) override;
    LineAdder newLine() override;
    LineAdder newLine(const std::string& subNetworkId) override;
    LineAdder newLine(const Line& copyLine) override;
    LineAdder newLine(const std::string& subNetworkId, const Line& copyLine) override;
    Network& newSubnetwork(const std::string& id, const std::string& sourceFormat) override;
    SubstationAdder newSubstation() override;
    TieLineAdder newTieLine() override;
    TieLineAdder newTieLine(const std::string& subNetworkId) override;
    VoltageAngleLimitAdder newVoltageAngleLimit() override;
    VoltageAngleLimitAdder newVoltageAngleLimit(const std::string& subNetworkId) override;
    VoltageLevelAdder newVoltageLevel() override;

    ValidationLevel runValidationChecks() override;
    ValidationLevel runValidationChecks(const ValidationLevel& vl) override;
    const ValidationLevel& validate() override;
    ValidationLevel getValidationLevel() const override;
    Network& setMinimumAcceptableValidationLevel(const ValidationLevel& minimumValidationLevel) override;
    const ValidationLevel& getMinimumValidationLevel() const override;
    Network& setValidationLevelIfGreaterThan(const ValidationLevel& vl) override;
    Network& invalidateValidationLevel() override;

protected: //MultiVariantObject
    const network::VariantArray& getVariants() const override;
    network::VariantArray& getVariants() override;

protected:
    const NetworkIndex& getIndex() const override;
    NetworkIndex& getIndex() override;

private:
    subnetwork::BusBreakerView m_subBusBreakerView;

    subnetwork::BusView m_subBusView;

    std::function<bool(const Identifiable&)> m_filterIdentifiable;
    std::function<bool(const VoltageAngleLimit&)> m_filterVoltageAngleLimit;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_SUBNETWORK_HPP