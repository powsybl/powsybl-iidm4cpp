/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_NETWORK_HPP
#define POWSYBL_IIDM_NETWORK_HPP

#include <boost/filesystem/path.hpp>

#include <powsybl/iidm/AreaAdder.hpp>
#include <powsybl/iidm/Container.hpp>
#include <powsybl/iidm/NetworkIndex.hpp>
#include <powsybl/iidm/NetworkRef.hpp>
#include <powsybl/iidm/NetworkVariant.hpp>
#include <powsybl/iidm/NetworkViews.hpp>
#include <powsybl/iidm/SubnetworkViews.hpp>
#include <powsybl/iidm/SubstationAdder.hpp>
#include <powsybl/iidm/ValidationLevel.hpp>
#include <powsybl/iidm/VariantArray.hpp>
#include <powsybl/iidm/VariantManager.hpp>
#include <powsybl/iidm/VariantManagerHolder.hpp>
#include <powsybl/iidm/VoltageAngleLimit.hpp>
#include <powsybl/iidm/converter/Constants.hpp>
#include <powsybl/iidm/converter/ExportOptions.hpp>
#include <powsybl/iidm/converter/ImportOptions.hpp>
#include <powsybl/stdcxx/DateTime.hpp>
#include <powsybl/stdcxx/range.hpp>

#include <map>

namespace powsybl {

namespace iidm {

class AreaAdder;
class Battery;
class Branch;
class BusCache;
class BusbarSection;
class Connectable;
class ConnectedComponentsManager;
class DanglingLine;
class Generator;
class Ground;
class HvdcConverterStation;
class HvdcLine;
class HvdcLineAdder;
class LccConverterStation;
class Line;
class LineAdder;
class Load;
class OverloadManagementSystem;
class ShuntCompensator;
class StaticVarCompensator;
class Substation;
class SubstationAdder;
class Switch;
class SynchronousComponentsManager;
class ThreeWindingsTransformer;
class ThreeWindingsTransformerAdder;
class TieLine;
class TieLineAdder;
class TwoWindingsTransformer;
class TwoWindingsTransformerAdder;
class VoltageAngleLimitAdder;
class VoltageLevel;
class VoltageLevelAdder;
class VscConverterStation;

class Network : public Container, public VariantManagerHolder {
public:
    using BusBreakerView = network::BusBreakerView;

    using BusView = network::BusView;

public:
    static Network readXml(const boost::filesystem::path& path, const converter::ImportOptions& options = converter::ImportOptions());

    static Network readXml(const std::string& filename, std::istream& istream, const converter::ImportOptions& options = converter::ImportOptions());

    static void writeXml(const boost::filesystem::path& path, const Network& network, const converter::ExportOptions& options = converter::ExportOptions());

    static void writeXml(const std::string& filename, std::ostream& ostream, const Network& network, const converter::ExportOptions& options = converter::ExportOptions());

public:  // Identifiable
    virtual const Network& getNetwork() const override;
    virtual Network& getNetwork() override;

    stdcxx::CReference<Network> getParentNetworkRef() const;
    stdcxx::Reference<Network> getParentNetworkRef();

    const Network& getRootNetwork() const;
    Network& getRootNetwork();

    bool contains(const Identifiable& identifiable) const;
    bool contains(Identifiable& identifiable) const;

    const IdentifiableType& getType() const override;

public:  // VariantManagerHolder
    virtual unsigned long getVariantIndex() const override;

    virtual const VariantManager& getVariantManager() const override;

    virtual VariantManager& getVariantManager() override;

protected:
    /**
    * Same as default constructor, with additional bool to control adding this network in the NetworkIndex, (for subnetwork)
    */
    Network(const std::string& id, const std::string& sourceFormat, bool manageIndex);
public:
    Network(const std::string& id, const std::string& sourceFormat);

    Network(const Network&) = delete;

    Network(Network&& network) noexcept;

    ~Network() noexcept override = default;

    Network& operator=(const Network&) = delete;

    Network& operator=(Network&&) noexcept = delete;

    template <typename T>
    T& checkAndAdd(std::unique_ptr<T>&& identifiable);

    template <typename T = Identifiable, typename = typename std::enable_if<std::is_base_of<Identifiable, T>::value>::type>
    stdcxx::CReference<T> find(const std::string& id) const;

    template <typename T = Identifiable, typename = typename std::enable_if<std::is_base_of<Identifiable, T>::value>::type>
    stdcxx::Reference<T> find(const std::string& id);

    virtual stdcxx::CReference<HvdcLine> findHvdcLine(const HvdcConverterStation& station) const;

    virtual stdcxx::Reference<HvdcLine> findHvdcLine(const HvdcConverterStation& station);

    template <typename T = Identifiable, typename = typename std::enable_if<std::is_base_of<Identifiable, T>::value>::type>
    const T& get(const std::string& id) const;

    template <typename T = Identifiable, typename = typename std::enable_if<std::is_base_of<Identifiable, T>::value>::type>
    T& get(const std::string& id);

    virtual unsigned long getAreaTypeCount() const;

    virtual std::set<std::string> getAreaTypes() const;

    virtual unsigned long getAreaCount() const;

    virtual stdcxx::const_range<Area> getAreas() const;

    virtual stdcxx::range<Area> getAreas();

    virtual const Area& getArea(const std::string& id) const;

    virtual Area& getArea(const std::string& id);

    virtual const Battery& getBattery(const std::string& id) const;

    virtual Battery& getBattery(const std::string& id);

    virtual unsigned long getBatteryCount() const;

    virtual stdcxx::const_range<Battery> getBatteries() const;

    virtual stdcxx::range<Battery> getBatteries();

    virtual const Branch& getBranch(const std::string& id) const;

    virtual Branch& getBranch(const std::string& id);

    virtual unsigned long getBranchCount() const;

    virtual stdcxx::const_range<Branch> getBranches() const;

    virtual stdcxx::range<Branch> getBranches();

    virtual const BusbarSection& getBusbarSection(const std::string& id) const;

    virtual BusbarSection& getBusbarSection(const std::string& id);

    virtual unsigned long getBusbarSectionCount() const;

    virtual stdcxx::const_range<BusbarSection> getBusbarSections() const;

    virtual stdcxx::range<BusbarSection> getBusbarSections();

    virtual const BusBreakerView& getBusBreakerView() const;

    virtual BusBreakerView& getBusBreakerView();

    virtual const BusView& getBusView() const;

    virtual BusView& getBusView();

    const stdcxx::DateTime& getCaseDate() const;

    template <typename T = Connectable, typename = typename std::enable_if<std::is_base_of<Connectable, T>::value>::type>
    unsigned long getConnectableCount() const;

    template <typename T = Connectable, typename = typename std::enable_if<std::is_base_of<Connectable, T>::value>::type>
    stdcxx::const_range<T> getConnectables() const;

    template <typename T = Connectable, typename = typename std::enable_if<std::is_base_of<Connectable, T>::value>::type>
    stdcxx::range<T> getConnectables();

    const ConnectedComponentsManager& getConnectedComponentsManager() const;

    ConnectedComponentsManager& getConnectedComponentsManager();

    virtual std::set<Country> getCountries() const;

    virtual unsigned long getCountryCount() const;

    virtual const DanglingLine& getDanglingLine(const std::string& id) const;

    virtual DanglingLine& getDanglingLine(const std::string& id);

    virtual unsigned long getDanglingLineCount() const;

    virtual stdcxx::const_range<DanglingLine> getDanglingLines(const DanglingLineFilter& filter) const;

    virtual stdcxx::range<DanglingLine> getDanglingLines(const DanglingLineFilter& filter);

    virtual stdcxx::const_range<DanglingLine> getDanglingLines() const;

    virtual stdcxx::range<DanglingLine> getDanglingLines();

    int getForecastDistance() const;

    virtual const Generator& getGenerator(const std::string& id) const;

    virtual Generator& getGenerator(const std::string& id);

    virtual unsigned long getGeneratorCount() const;

    virtual stdcxx::const_range<Generator> getGenerators() const;

    virtual stdcxx::range<Generator> getGenerators();

    virtual const Ground& getGround(const std::string& id) const;

    virtual Ground& getGround(const std::string& id);

    virtual unsigned long getGroundCount() const;

    virtual stdcxx::const_range<Ground> getGrounds() const;

    virtual stdcxx::range<Ground> getGrounds();

    virtual const HvdcConverterStation& getHvdcConverterStation(const std::string& id) const;

    virtual HvdcConverterStation& getHvdcConverterStation(const std::string& id);

    virtual unsigned long getHvdcConverterStationCount() const;

    virtual stdcxx::const_range<HvdcConverterStation> getHvdcConverterStations() const;

    virtual stdcxx::range<HvdcConverterStation> getHvdcConverterStations();

    virtual const HvdcLine& getHvdcLine(const std::string& id) const;

    virtual HvdcLine& getHvdcLine(const std::string& id);

    virtual const HvdcLine& getHvdcLine(const HvdcConverterStation& station) const;

    virtual HvdcLine& getHvdcLine(const HvdcConverterStation& station);

    virtual unsigned long getHvdcLineCount() const;

    virtual stdcxx::const_range<HvdcLine> getHvdcLines() const;

    virtual stdcxx::range<HvdcLine> getHvdcLines();

    virtual const Identifiable& getIdentifiable(const std::string& id) const;

    virtual Identifiable& getIdentifiable(const std::string& id);

    virtual stdcxx::const_range<Identifiable> getIdentifiables() const;

    virtual stdcxx::range<Identifiable> getIdentifiables();

    virtual const LccConverterStation& getLccConverterStation(const std::string& id) const;

    virtual LccConverterStation& getLccConverterStation(const std::string& id);

    virtual unsigned long getLccConverterStationCount() const;

    virtual stdcxx::const_range<LccConverterStation> getLccConverterStations() const;

    virtual stdcxx::range<LccConverterStation> getLccConverterStations();

    virtual const Line& getLine(const std::string& id) const;

    virtual Line& getLine(const std::string& id);

    virtual unsigned long getLineCount() const;

    virtual stdcxx::const_range<Line> getLines() const;

    virtual stdcxx::range<Line> getLines();

    virtual const TieLine& getTieLine(const std::string& id) const;

    virtual TieLine& getTieLine(const std::string& id);

    virtual unsigned long getTieLineCount() const;

    virtual stdcxx::const_range<TieLine> getTieLines() const;

    virtual stdcxx::range<TieLine> getTieLines();

    virtual const Load& getLoad(const std::string& id) const;

    virtual Load& getLoad(const std::string& id);

    virtual unsigned long getLoadCount() const;

    virtual stdcxx::const_range<Load> getLoads() const;

    virtual stdcxx::range<Load> getLoads();

    virtual const ValidationLevel& getMinimumValidationLevel() const;

    virtual const OverloadManagementSystem& getOverloadManagementSystem(const std::string& id) const;

    virtual OverloadManagementSystem& getOverloadManagementSystem(const std::string& id);

    virtual unsigned long getOverloadManagementSystemCount() const;

    virtual stdcxx::const_range<OverloadManagementSystem> getOverloadManagementSystems() const;

    virtual stdcxx::range<OverloadManagementSystem> getOverloadManagementSystems();

    virtual const ShuntCompensator& getShuntCompensator(const std::string& id) const;

    virtual ShuntCompensator& getShuntCompensator(const std::string& id);

    virtual unsigned long getShuntCompensatorCount() const;

    virtual stdcxx::const_range<ShuntCompensator> getShuntCompensators() const;

    virtual stdcxx::range<ShuntCompensator> getShuntCompensators();

    const std::string& getSourceFormat() const;

    stdcxx::const_range<MultiVariantObject> getStatefulObjects() const;

    stdcxx::range<MultiVariantObject> getStatefulObjects();

    virtual const StaticVarCompensator& getStaticVarCompensator(const std::string& id) const;

    virtual StaticVarCompensator& getStaticVarCompensator(const std::string& id);

    virtual unsigned long getStaticVarCompensatorCount() const;

    virtual stdcxx::const_range<StaticVarCompensator> getStaticVarCompensators() const;

    virtual stdcxx::range<StaticVarCompensator> getStaticVarCompensators();

    virtual stdcxx::CReference<Network> getSubNetwork(const std::string& id) const;

    virtual stdcxx::Reference<Network> getSubNetwork(const std::string& id);

    virtual unsigned long getSubNetworksCount() const;

    virtual stdcxx::const_range<Network> getSubNetworks() const;

    virtual stdcxx::range<Network> getSubNetworks();

    virtual const Substation& getSubstation(const std::string& id) const;

    virtual Substation& getSubstation(const std::string& id);

    virtual unsigned long getSubstationCount() const;

    virtual stdcxx::const_range<Substation> getSubstations() const;

    virtual stdcxx::range<Substation> getSubstations();

    virtual const Switch& getSwitch(const std::string& id) const;

    virtual Switch& getSwitch(const std::string& id);

    virtual unsigned long getSwitchCount() const;

    virtual stdcxx::const_range<Switch> getSwitches() const;

    virtual stdcxx::range<Switch> getSwitches();

    const SynchronousComponentsManager& getSynchronousComponentsManager() const;

    SynchronousComponentsManager& getSynchronousComponentsManager();

    virtual const ThreeWindingsTransformer& getThreeWindingsTransformer(const std::string& id) const;

    virtual ThreeWindingsTransformer& getThreeWindingsTransformer(const std::string& id);

    virtual unsigned long getThreeWindingsTransformerCount() const;

    virtual stdcxx::const_range<ThreeWindingsTransformer> getThreeWindingsTransformers() const;

    virtual stdcxx::range<ThreeWindingsTransformer> getThreeWindingsTransformers();

    virtual const TwoWindingsTransformer& getTwoWindingsTransformer(const std::string& id) const;

    virtual TwoWindingsTransformer& getTwoWindingsTransformer(const std::string& id);

    virtual unsigned long getTwoWindingsTransformerCount() const;

    virtual stdcxx::const_range<TwoWindingsTransformer> getTwoWindingsTransformers() const;

    virtual stdcxx::range<TwoWindingsTransformer> getTwoWindingsTransformers();

    virtual const VoltageAngleLimit& getVoltageAngleLimit(const std::string& id) const;

    virtual VoltageAngleLimit& getVoltageAngleLimit(const std::string& id);

    virtual unsigned long getVoltageAngleLimitsCount() const;

    virtual stdcxx::const_range<VoltageAngleLimit> getVoltageAngleLimits() const;

    virtual stdcxx::range<VoltageAngleLimit> getVoltageAngleLimits();

    virtual std::map<std::string, VoltageAngleLimit>& getVoltageAngleLimitsIndex();

    virtual const VoltageLevel& getVoltageLevel(const std::string& id) const;

    virtual VoltageLevel& getVoltageLevel(const std::string& id);

    virtual unsigned long getVoltageLevelCount() const;

    virtual stdcxx::const_range<VoltageLevel> getVoltageLevels() const;

    virtual stdcxx::range<VoltageLevel> getVoltageLevels();

    virtual const VscConverterStation& getVscConverterStation(const std::string& id) const;

    virtual VscConverterStation& getVscConverterStation(const std::string& id);

    virtual unsigned long getVscConverterStationCount() const;

    virtual stdcxx::const_range<VscConverterStation> getVscConverterStations() const;

    virtual stdcxx::range<VscConverterStation> getVscConverterStations();

    virtual AreaAdder newArea();

    virtual HvdcLineAdder newHvdcLine();
    virtual HvdcLineAdder newHvdcLine(const std::string& subNetworkId);

    virtual LineAdder newLine();
    virtual LineAdder newLine(const std::string& subNetworkId);

    virtual LineAdder newLine(const Line& copyLine);
    virtual LineAdder newLine(const std::string& subNetworkId, const Line& copyLine);

    /**
     * Creates a new subnetwork inside this current network;
     * @return created subnetwork
     */
    virtual Network& newSubnetwork(const std::string& id, const std::string& sourceFormat);

    virtual SubstationAdder newSubstation();

    virtual TieLineAdder newTieLine();
    virtual TieLineAdder newTieLine(const std::string& subNetworkId);

    virtual VoltageAngleLimitAdder newVoltageAngleLimit();
    virtual VoltageAngleLimitAdder newVoltageAngleLimit(const std::string& subNetworkId);

    virtual VoltageLevelAdder newVoltageLevel();

    virtual void remove(Identifiable& identifiable);

    Network& setCaseDate(const stdcxx::DateTime& caseDate);

    Network& setForecastDistance(int forecastDistance);

    /*
    * If network is valid, does nothing.
    * Else, runs a validation check on each network component. 
    * Exception is thrown if one component is not valid.
    * Network validation status id updated.
    * @return resulting ValidationLevel of the network
    */
    virtual ValidationLevel runValidationChecks();
    /*
    * If network is valid, does nothing.
    * Else, runs a validation check on each network component. 
    * If given ValidationLevel is STEADY_SATE_HYPOTHESIS, exception is thrown if one component is not valid.
    * Network validation status id updated.
    * @return resulting ValidationLevel of the network
    */
    virtual ValidationLevel runValidationChecks(const ValidationLevel& vl);

    /*
    * If network validation level not evaluated, runs validation checks and updates network validation level.
    */
    virtual const ValidationLevel& validate();
    /*
    * If network validation level not evaluated, runs validation checks.
    * Returns network's validation level. 
    * Does NOT update network validation level value.
    */
    virtual ValidationLevel getValidationLevel() const;

    /*
    * Set the minimum validation level of the network
    * If the given validation level value is not matched by the current network validation status, an Exception is thrown.
    */
    virtual Network& setMinimumAcceptableValidationLevel(const ValidationLevel& minimumValidationLevel);

    /*
    * Update networtk validation level to the minimum between current status and given validation level
    */
    virtual Network& setValidationLevelIfGreaterThan(const ValidationLevel& vl);
    /*
    * If minimum validation level is STEADY STATE HYPOTHESIS, does nothing.
    * Else, unvalid the network validation level status. A new check will have to be performed.
    */
    virtual Network& invalidateValidationLevel();


protected:  // MultiVariantObject
    void allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) override;

    void deleteVariantArrayElement(unsigned long index) override;

    void extendVariantArraySize(unsigned long initVariantArraySize, unsigned long number, unsigned long sourceIndex) override;

    void reduceVariantArraySize(unsigned long number) override;

    virtual const network::VariantArray& getVariants() const;
    virtual network::VariantArray& getVariants();

protected:
    const BusCache& getBusBreakerViewCache() const;

    BusCache& getBusBreakerViewCache();

    const BusCache& getBusViewCache() const;

    BusCache& getBusViewCache();

    template <typename T, typename = typename std::enable_if<std::is_base_of<Identifiable, T>::value>::type>
    unsigned long getObjectCount() const;

    virtual const NetworkIndex& getIndex() const;

    virtual NetworkIndex& getIndex();

private:  // Identifiable
    const std::string& getTypeDescription() const override;

private:
    stdcxx::DateTime m_caseDate;

    int m_forecastDistance = 0;

    std::string m_sourceFormat;

    ValidationLevel m_minimumValidationLevel{ValidationLevel::STEADY_STATE_HYPOTHESIS};
    ValidationLevel m_validationLevel{ValidationLevel::UNVALID};

    NetworkIndex m_networkIndex;

    VariantManager m_variantManager;

    network::VariantArray m_variants;

    BusBreakerView m_busBreakerView;

    BusView m_busView;

    std::map<std::string, VoltageAngleLimit> m_voltageAngleLimitsIndex;

    stdcxx::Reference<Network> m_parentNetworkRef;
    std::map<std::string, stdcxx::Reference<Network>> m_subNetworksIndex;

    friend class Identifiable;

    friend class VoltageLevel;

    friend class NodeBreakerTopologyModel;

    friend class network::BusView;
    friend class subnetwork::BusView;

    friend class network::BusBreakerView;
    friend class subnetwork::BusBreakerView;

    friend class Subnetwork;
};

}  // namespace iidm

}  // namespace powsybl

#include <powsybl/iidm/Network.hxx>

#endif  // POWSYBL_IIDM_NETWORK_HPP
