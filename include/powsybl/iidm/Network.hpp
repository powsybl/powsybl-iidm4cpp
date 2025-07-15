/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_NETWORK_HPP
#define POWSYBL_IIDM_NETWORK_HPP

#include <boost/filesystem/path.hpp>

#include <powsybl/iidm/Container.hpp>
#include <powsybl/iidm/NetworkIndex.hpp>
#include <powsybl/iidm/NetworkVariant.hpp>
#include <powsybl/iidm/NetworkViews.hpp>
#include <powsybl/iidm/ValidationLevel.hpp>
#include <powsybl/iidm/SubstationAdder.hpp>
#include <powsybl/iidm/VariantArray.hpp>
#include <powsybl/iidm/VariantManager.hpp>
#include <powsybl/iidm/VariantManagerHolder.hpp>
#include <powsybl/iidm/converter/Constants.hpp>
#include <powsybl/iidm/converter/ExportOptions.hpp>
#include <powsybl/iidm/converter/ImportOptions.hpp>
#include <powsybl/stdcxx/DateTime.hpp>
#include <powsybl/stdcxx/range.hpp>

namespace powsybl {

namespace iidm {

class Battery;
class Branch;
class BusCache;
class BusbarSection;
class Connectable;
class ConnectedComponentsManager;
class DanglingLine;
class Generator;
class HvdcConverterStation;
class HvdcLine;
class HvdcLineAdder;
class LccConverterStation;
class Line;
class LineAdder;
class Load;
class ShuntCompensator;
class StaticVarCompensator;
class Substation;
class SubstationAdder;
class Switch;
class SynchronousComponentsManager;
class ThreeWindingsTransformer;
class ThreeWindingsTransformerAdder;
class TieLineAdder;
class TwoWindingsTransformer;
class TwoWindingsTransformerAdder;
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
    const Network& getNetwork() const override;

    Network& getNetwork() override;

    const IdentifiableType& getType() const override;

public:  // VariantManagerHolder
    unsigned long getVariantIndex() const override;

    const VariantManager& getVariantManager() const override;

    VariantManager& getVariantManager() override;

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

    stdcxx::CReference<HvdcLine> findHvdcLine(const HvdcConverterStation& station) const;

    stdcxx::Reference<HvdcLine> findHvdcLine(const HvdcConverterStation& station);

    template <typename T = Identifiable, typename = typename std::enable_if<std::is_base_of<Identifiable, T>::value>::type>
    const T& get(const std::string& id) const;

    template <typename T = Identifiable, typename = typename std::enable_if<std::is_base_of<Identifiable, T>::value>::type>
    T& get(const std::string& id);

    const Battery& getBattery(const std::string& id) const;

    Battery& getBattery(const std::string& id);

    unsigned long getBatteryCount() const;

    stdcxx::const_range<Battery> getBatteries() const;

    stdcxx::range<Battery> getBatteries();

    const Branch& getBranch(const std::string& id) const;

    Branch& getBranch(const std::string& id);

    unsigned long getBranchCount() const;

    stdcxx::const_range<Branch> getBranches() const;

    stdcxx::range<Branch> getBranches();

    const BusbarSection& getBusbarSection(const std::string& id) const;

    BusbarSection& getBusbarSection(const std::string& id);

    unsigned long getBusbarSectionCount() const;

    stdcxx::const_range<BusbarSection> getBusbarSections() const;

    stdcxx::range<BusbarSection> getBusbarSections();

    const BusBreakerView& getBusBreakerView() const;

    BusBreakerView& getBusBreakerView();

    const BusView& getBusView() const;

    BusView& getBusView();

    const stdcxx::DateTime& getCaseDate() const;

    template <typename T = Connectable, typename = typename std::enable_if<std::is_base_of<Connectable, T>::value>::type>
    unsigned long getConnectableCount() const;

    template <typename T = Connectable, typename = typename std::enable_if<std::is_base_of<Connectable, T>::value>::type>
    stdcxx::const_range<T> getConnectables() const;

    template <typename T = Connectable, typename = typename std::enable_if<std::is_base_of<Connectable, T>::value>::type>
    stdcxx::range<T> getConnectables();

    const ConnectedComponentsManager& getConnectedComponentsManager() const;

    ConnectedComponentsManager& getConnectedComponentsManager();

    std::set<Country> getCountries() const;

    unsigned long getCountryCount() const;

    const DanglingLine& getDanglingLine(const std::string& id) const;

    DanglingLine& getDanglingLine(const std::string& id);

    unsigned long getDanglingLineCount() const;

    stdcxx::const_range<DanglingLine> getDanglingLines() const;

    stdcxx::range<DanglingLine> getDanglingLines();

    int getForecastDistance() const;

    const Generator& getGenerator(const std::string& id) const;

    Generator& getGenerator(const std::string& id);

    unsigned long getGeneratorCount() const;

    stdcxx::const_range<Generator> getGenerators() const;

    stdcxx::range<Generator> getGenerators();

    const HvdcConverterStation& getHvdcConverterStation(const std::string& id) const;

    HvdcConverterStation& getHvdcConverterStation(const std::string& id);

    unsigned long getHvdcConverterStationCount() const;

    stdcxx::const_range<HvdcConverterStation> getHvdcConverterStations() const;

    stdcxx::range<HvdcConverterStation> getHvdcConverterStations();

    const HvdcLine& getHvdcLine(const std::string& id) const;

    HvdcLine& getHvdcLine(const std::string& id);

    const HvdcLine& getHvdcLine(const HvdcConverterStation& station) const;

    HvdcLine& getHvdcLine(const HvdcConverterStation& station);

    unsigned long getHvdcLineCount() const;

    stdcxx::const_range<HvdcLine> getHvdcLines() const;

    stdcxx::range<HvdcLine> getHvdcLines();

    const Identifiable& getIdentifiable(const std::string& id) const;

    Identifiable& getIdentifiable(const std::string& id);

    stdcxx::const_range<Identifiable> getIdentifiables() const;

    stdcxx::range<Identifiable> getIdentifiables();

    const LccConverterStation& getLccConverterStation(const std::string& id) const;

    LccConverterStation& getLccConverterStation(const std::string& id);

    unsigned long getLccConverterStationCount() const;

    stdcxx::const_range<LccConverterStation> getLccConverterStations() const;

    stdcxx::range<LccConverterStation> getLccConverterStations();

    const Line& getLine(const std::string& id) const;

    Line& getLine(const std::string& id);

    unsigned long getLineCount() const;

    stdcxx::const_range<Line> getLines() const;

    stdcxx::range<Line> getLines();

    const Load& getLoad(const std::string& id) const;

    Load& getLoad(const std::string& id);

    unsigned long getLoadCount() const;

    stdcxx::const_range<Load> getLoads() const;

    stdcxx::range<Load> getLoads();

    const ValidationLevel& getMinimumValidationLevel() const;

    const ShuntCompensator& getShuntCompensator(const std::string& id) const;

    ShuntCompensator& getShuntCompensator(const std::string& id);

    unsigned long getShuntCompensatorCount() const;

    stdcxx::const_range<ShuntCompensator> getShuntCompensators() const;

    stdcxx::range<ShuntCompensator> getShuntCompensators();

    const std::string& getSourceFormat() const;

    stdcxx::const_range<MultiVariantObject> getStatefulObjects() const;

    stdcxx::range<MultiVariantObject> getStatefulObjects();

    const StaticVarCompensator& getStaticVarCompensator(const std::string& id) const;

    StaticVarCompensator& getStaticVarCompensator(const std::string& id);

    unsigned long getStaticVarCompensatorCount() const;

    stdcxx::const_range<StaticVarCompensator> getStaticVarCompensators() const;

    stdcxx::range<StaticVarCompensator> getStaticVarCompensators();

    const Substation& getSubstation(const std::string& id) const;

    Substation& getSubstation(const std::string& id);

    unsigned long getSubstationCount() const;

    stdcxx::const_range<Substation> getSubstations() const;

    stdcxx::range<Substation> getSubstations();

    const Switch& getSwitch(const std::string& id) const;

    Switch& getSwitch(const std::string& id);

    unsigned long getSwitchCount() const;

    stdcxx::const_range<Switch> getSwitches() const;

    stdcxx::range<Switch> getSwitches();

    const SynchronousComponentsManager& getSynchronousComponentsManager() const;

    SynchronousComponentsManager& getSynchronousComponentsManager();

    const ThreeWindingsTransformer& getThreeWindingsTransformer(const std::string& id) const;

    ThreeWindingsTransformer& getThreeWindingsTransformer(const std::string& id);

    unsigned long getThreeWindingsTransformerCount() const;

    stdcxx::const_range<ThreeWindingsTransformer> getThreeWindingsTransformers() const;

    stdcxx::range<ThreeWindingsTransformer> getThreeWindingsTransformers();

    const TwoWindingsTransformer& getTwoWindingsTransformer(const std::string& id) const;

    TwoWindingsTransformer& getTwoWindingsTransformer(const std::string& id);

    unsigned long getTwoWindingsTransformerCount() const;

    stdcxx::const_range<TwoWindingsTransformer> getTwoWindingsTransformers() const;

    stdcxx::range<TwoWindingsTransformer> getTwoWindingsTransformers();

    const VoltageLevel& getVoltageLevel(const std::string& id) const;

    VoltageLevel& getVoltageLevel(const std::string& id);

    unsigned long getVoltageLevelCount() const;

    stdcxx::const_range<VoltageLevel> getVoltageLevels() const;

    stdcxx::range<VoltageLevel> getVoltageLevels();

    const VscConverterStation& getVscConverterStation(const std::string& id) const;

    VscConverterStation& getVscConverterStation(const std::string& id);

    unsigned long getVscConverterStationCount() const;

    stdcxx::const_range<VscConverterStation> getVscConverterStations() const;

    stdcxx::range<VscConverterStation> getVscConverterStations();

    HvdcLineAdder newHvdcLine();

    LineAdder newLine();

    SubstationAdder newSubstation();

    ThreeWindingsTransformerAdder newThreeWindingsTransformer();

    TieLineAdder newTieLine();

    TwoWindingsTransformerAdder newTwoWindingsTransformer();

    VoltageLevelAdder newVoltageLevel();

    void remove(Identifiable& identifiable);

    Network& setCaseDate(const stdcxx::DateTime& caseDate);

    Network& setForecastDistance(int forecastDistance);

    ValidationLevel runValidationChecks();
    ValidationLevel runValidationChecks(const ValidationLevel& vl);

    const ValidationLevel& getValidationLevel();

    Network& setMinimumValidationLevel(const ValidationLevel& minimumValidationLevel);
    Network& setMinimumAcceptableValidationLevel(const ValidationLevel& vl);

    Network& setValidationLevelIfGreaterThan(const ValidationLevel& vl);
    Network& invalidateValidationLevel();

protected:  // MultiVariantObject
    void allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) override;

    void deleteVariantArrayElement(unsigned long index) override;

    void extendVariantArraySize(unsigned long initVariantArraySize, unsigned long number, unsigned long sourceIndex) override;

    void reduceVariantArraySize(unsigned long number) override;

private:  // Identifiable
    const std::string& getTypeDescription() const override;

private:
    const BusCache& getBusBreakerViewCache() const;

    BusCache& getBusBreakerViewCache();

    const BusCache& getBusViewCache() const;

    BusCache& getBusViewCache();

    template <typename T, typename = typename std::enable_if<std::is_base_of<Identifiable, T>::value>::type>
    unsigned long getObjectCount() const;

    const NetworkIndex& getIndex() const;

    NetworkIndex& getIndex();

    friend class Identifiable;

    friend class VoltageLevel;

    friend class NodeBreakerVoltageLevel;

    friend class network::BusView;

    friend class network::BusBreakerView;

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
};

}  // namespace iidm

}  // namespace powsybl

#include <powsybl/iidm/Network.hxx>

#endif  // POWSYBL_IIDM_NETWORK_HPP
