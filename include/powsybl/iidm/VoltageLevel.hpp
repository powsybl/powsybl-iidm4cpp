/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_VOLTAGELEVEL_HPP
#define POWSYBL_IIDM_VOLTAGELEVEL_HPP

#include <set>

#include <powsybl/iidm/Connectable.hpp>
#include <powsybl/iidm/Container.hpp>
#include <powsybl/iidm/NetworkRef.hpp>
#include <powsybl/iidm/TerminalSet.hpp>
#include <powsybl/iidm/TopologyKind.hpp>
#include <powsybl/iidm/TopologyModel.hpp>
#include <powsybl/iidm/VoltageLevelViews.hpp>
#include <powsybl/stdcxx/range.hpp>
#include <powsybl/stdcxx/Predicate.hpp>
#include <powsybl/stdcxx/reference.hpp>

namespace powsybl {

namespace iidm {

class Area;
class Battery;
class BatteryAdder;
class BoundaryLine;
class BoundaryLineAdder;
class Generator;
class GeneratorAdder;
class Ground;
class GroundAdder;
class LineCommutatedConverter;
class LineCommutatedConverterAdder;
class LccConverterStation;
class LccConverterStationAdder;
class Load;
class LoadAdder;
class Network;
class NetworkIndex;
class ShuntCompensator;
class ShuntCompensatorAdder;
class StaticVarCompensator;
class StaticVarCompensatorAdder;
class Switch;
class Substation;
class Terminal;
class TopologyVisitor;
class VoltageSourceConverter;
class VoltageSourceConverterAdder;
class VscConverterStation;
class VscConverterStationAdder;

class VoltageLevel : public Container {
public:
    using BusBreakerView = voltage_level::BusBreakerView;

    using BusView = voltage_level::BusView;

    using NodeBreakerView = voltage_level::NodeBreakerView;

public:  // Identifiable
    const Network& getNetwork() const override;

    Network& getNetwork() override;

    const Network& getParentNetwork() const override;

    Network& getParentNetwork() override;

    std::string getSubnetworkId() const;

    const IdentifiableType& getType() const override;

public:
    ~VoltageLevel() noexcept override = default;



    unsigned long getAreaCount() const;
    stdcxx::const_range<Area> getAreas() const;
    stdcxx::range<Area> getAreas();
    stdcxx::CReference<Area> getArea(const std::string& areaType) const;
    stdcxx::Reference<Area> getArea(const std::string& areaType);

    void addArea(Area& area);
    void removeArea(Area& area);

    unsigned long getBatteryCount() const;

    stdcxx::const_range<Battery> getBatteries() const;

    stdcxx::range<Battery> getBatteries();

    const BusBreakerView& getBusBreakerView() const;

    BusBreakerView& getBusBreakerView();

    const BusView& getBusView() const;

    BusView& getBusView();

    template <typename T, typename = typename std::enable_if<std::is_base_of<Connectable, T>::value>::type>
    stdcxx::CReference<T> getConnectable(const std::string& id) const;

    template <typename T, typename = typename std::enable_if<std::is_base_of<Connectable, T>::value>::type>
    stdcxx::Reference<T> getConnectable(const std::string& id);

    template <typename T = Connectable, typename = typename std::enable_if<std::is_base_of<Connectable, T>::value>::type>
    unsigned long getConnectableCount() const;

    template <typename T = Connectable, typename = typename std::enable_if<std::is_base_of<Connectable, T>::value>::type>
    stdcxx::const_range<T> getConnectables() const;

    template <typename T = Connectable, typename = typename std::enable_if<std::is_base_of<Connectable, T>::value>::type>
    stdcxx::range<T> getConnectables();

    unsigned long getBoundaryLineCount() const;

    stdcxx::const_range<BoundaryLine> getBoundaryLines() const;

    stdcxx::range<BoundaryLine> getBoundaryLines();

    stdcxx::const_range<BoundaryLine> getBoundaryLines(const BoundaryLineFilter& filter) const;

    stdcxx::range<BoundaryLine> getBoundaryLines(const BoundaryLineFilter& filter);

    unsigned long getGeneratorCount() const;

    stdcxx::const_range<Generator> getGenerators() const;

    stdcxx::range<Generator> getGenerators();

    unsigned long getGroundCount() const;

    stdcxx::const_range<Ground> getGrounds() const;

    stdcxx::range<Ground> getGrounds();

    double getHighVoltageLimit() const;

    unsigned long getLccConverterStationCount() const;

    stdcxx::const_range<LccConverterStation> getLccConverterStations() const;

    stdcxx::range<LccConverterStation> getLccConverterStations();

    unsigned long getLineCount() const;

    stdcxx::const_range<Line> getLines() const;

    stdcxx::range<Line> getLines();

    unsigned long getLineCommutatedConverterCount() const;

    stdcxx::const_range<LineCommutatedConverter> getLineCommutatedConverters() const;

    stdcxx::range<LineCommutatedConverter> getLineCommutatedConverters();

    unsigned long getLoadCount() const;

    stdcxx::const_range<Load> getLoads() const;

    stdcxx::range<Load> getLoads();

    double getLowVoltageLimit() const;

    const NodeBreakerView& getNodeBreakerView() const;

    NodeBreakerView& getNodeBreakerView();

    double getNominalV() const;

    unsigned long getShuntCompensatorCount() const;

    stdcxx::const_range<ShuntCompensator> getShuntCompensators() const;

    stdcxx::range<ShuntCompensator> getShuntCompensators();

    unsigned long getStaticVarCompensatorCount() const;

    stdcxx::const_range<StaticVarCompensator> getStaticVarCompensators() const;

    stdcxx::range<StaticVarCompensator> getStaticVarCompensators();

    stdcxx::CReference<Substation> getSubstation() const;

    stdcxx::Reference<Substation> getSubstation();

    unsigned long getSwitchCount() const;

    stdcxx::const_range<Switch> getSwitches() const;

    stdcxx::range<Switch> getSwitches();

    unsigned long getThreeWindingsTransformerCount() const;

    stdcxx::const_range<ThreeWindingsTransformer> getThreeWindingsTransformers() const;

    stdcxx::range<ThreeWindingsTransformer> getThreeWindingsTransformers();

    const TopologyKind& getTopologyKind() const;

    template <typename T = TopologyModel, typename = typename std::enable_if<std::is_base_of<TopologyModel, T>::value>::type>
    T& getTopologyModel();
    template <typename T = TopologyModel, typename = typename std::enable_if<std::is_base_of<TopologyModel, T>::value>::type>
    const T& getTopologyModel() const;

    unsigned long getTwoWindingsTransformerCount() const;

    stdcxx::const_range<TwoWindingsTransformer> getTwoWindingsTransformers() const;

    stdcxx::range<TwoWindingsTransformer> getTwoWindingsTransformers();

    unsigned long getVoltageSourceConverterCount() const;

    stdcxx::const_range<VoltageSourceConverter> getVoltageSourceConverters() const;

    stdcxx::range<VoltageSourceConverter> getVoltageSourceConverters();

    unsigned long getVscConverterStationCount() const;

    stdcxx::const_range<VscConverterStation> getVscConverterStations() const;

    stdcxx::range<VscConverterStation> getVscConverterStations();

    BatteryAdder newBattery();

    BoundaryLineAdder newBoundaryLine();

    GeneratorAdder newGenerator();

    GroundAdder newGround();

    LccConverterStationAdder newLccConverterStation();

    LineCommutatedConverterAdder newLineCommutatedConverter();

    LoadAdder newLoad();

    ShuntCompensatorAdder newShuntCompensator();

    StaticVarCompensatorAdder newStaticVarCompensator();

    VoltageSourceConverterAdder newVoltageSourceConverter();

    VscConverterStationAdder newVscConverterStation();

    void remove();

    VoltageLevel& setHighVoltageLimit(double highVoltageLimit);

    VoltageLevel& setLowVoltageLimit(double lowVoltageLimit);

    VoltageLevel& setNominalV(double nominalV);

    void visitEquipments(TopologyVisitor& visitor) const;

    /**
     * Convert topology model to the given kind.
     * When converting from node/breaker to bus/breaker, information are lost as during the process.
     * If current topology model is already of the given type, does nothing.
     * Only bus/breaker to node/breaker conversion is supported yet.
     */
    void convertToTopology(const TopologyKind& newTopologyKind);

protected:
    VoltageLevel(const std::string& id, const std::string& name, bool fictitious, const stdcxx::Reference<Substation>& substation,
                 Network& network, double nominalV, double lowVoltageLimit, double highVoltageLimit, const TopologyKind& topologyKind);
    VoltageLevel(const std::string& id, const std::string& name, bool fictitious, const stdcxx::Reference<Substation>& substation,
                 Network& rootNetwork, Network& subnetwork, double nominalV, double lowVoltageLimit, double highVoltageLimit, const TopologyKind& topologyKind);

    friend class VoltageLevelAdder;

protected:  // MultiVariantObject
    void allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) override;

    void deleteVariantArrayElement(unsigned long index) override;

    void extendVariantArraySize(unsigned long initVariantArraySize, unsigned long number, unsigned long sourceIndex) override;

    void reduceVariantArraySize(unsigned long number) override;

private: // Identifiable
    const std::string& getTypeDescription() const override;

private:
    void assertTopologyModel() const;

    void convertToBusBreakerTopology();

    void setNetworkRef(Network& network);

    friend class NetworkIndex;

private:
    NetworkRef m_network;
    stdcxx::Reference<Network> m_subnetworkRef;

    stdcxx::Reference<Substation> m_substation;

    double m_highVoltageLimit;

    double m_lowVoltageLimit;

    double m_nominalV;

    std::unique_ptr<TopologyModel> m_topologyModel;

    std::vector<stdcxx::Reference<Area>> m_areas;
};

}  // namespace iidm

}  // namespace powsybl

#include <powsybl/iidm/VoltageLevel.hxx>

#endif  // POWSYBL_IIDM_VOLTAGELEVEL_HPP
