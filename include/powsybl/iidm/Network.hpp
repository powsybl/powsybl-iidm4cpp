/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_NETWORK_HPP
#define POWSYBL_IIDM_NETWORK_HPP

#include <powsybl/iidm/Container.hpp>
#include <powsybl/iidm/NetworkIndex.hpp>
#include <powsybl/iidm/SubstationAdder.hpp>
#include <powsybl/iidm/VariantManager.hpp>
#include <powsybl/iidm/VariantManagerHolder.hpp>
#include <powsybl/iidm/bits/Network.hpp>

#include <powsybl/stdcxx/DateTime.hpp>

namespace powsybl {

namespace iidm {

class Battery;
class Branch;
class BusbarSection;
class Connectable;
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
class Switch;
class ThreeWindingsTransformer;
class TieLineAdder;
class TwoWindingsTransformer;
class VoltageLevel;
class VscConverterStation;

namespace converter {

class Anonymizer;
class ExportOptions;
class ImportOptions;

}  // namespace converter

class Network : public Container, public VariantManagerHolder {
public:
    static Network readXml(std::istream& istream);

    static Network readXml(std::istream& istream, const converter::ImportOptions& options, const stdcxx::CReference<converter::Anonymizer>& anonymizer);

    static std::unique_ptr<converter::Anonymizer> writeXml(std::ostream& ostream, const Network& network);

    static std::unique_ptr<converter::Anonymizer> writeXml(std::ostream& ostream, const Network& network, const converter::ExportOptions& options);

public:
    template <typename T>
    using range = typename network::range_traits<T>::range;

    template <typename T>
    using const_range = typename network::range_traits<T>::const_range;

public: // VariantManagerHolder
    unsigned long getVariantIndex() const override;

    const VariantManager& getVariantManager() const override;

    VariantManager& getVariantManager() override;

public:
    Network(const std::string& id, const std::string& sourceFormat);

    Network(Network&&) = default;

    ~Network() noexcept override = default;

    template <typename T>
    T& checkAndAdd(std::unique_ptr<T>&& identifiable);

    template <typename T, typename = typename std::enable_if<std::is_base_of<Identifiable, T>::value>::type>
    const T& get(const std::string& id) const;

    template <typename T, typename = typename std::enable_if<std::is_base_of<Identifiable, T>::value>::type>
    T& get(const std::string& id);

    const Battery& getBattery(const std::string& id) const;

    Battery& getBattery(const std::string& id);

    unsigned long getBatteryCount() const;

    const_range<Battery> getBatteries() const;

    range<Battery> getBatteries();

    const Branch& getBranch(const std::string& id) const;

    Branch& getBranch(const std::string& id);

    unsigned long getBranchCount() const;

    const_range<Branch> getBranches() const;

    range<Branch> getBranches();

    const BusbarSection& getBusbarSection(const std::string& id) const;

    BusbarSection& getBusbarSection(const std::string& id);

    unsigned long getBusbarSectionCount() const;

    const_range<BusbarSection> getBusbarSections() const;

    range<BusbarSection> getBusbarSections();

    const stdcxx::DateTime& getCaseDate() const;

    unsigned long getCountryCount() const;

    const DanglingLine& getDanglingLine(const std::string& id) const;

    DanglingLine& getDanglingLine(const std::string& id);

    unsigned long getDanglingLineCount() const;

    const_range<DanglingLine> getDanglingLines() const;

    range<DanglingLine> getDanglingLines();

    int getForecastDistance() const;

    const Generator& getGenerator(const std::string& id) const;

    Generator& getGenerator(const std::string& id);

    unsigned long getGeneratorCount() const;

    const_range<Generator> getGenerators() const;

    range<Generator> getGenerators();

    const HvdcConverterStation& getHvdcConverterStation(const std::string& id) const;

    HvdcConverterStation& getHvdcConverterStation(const std::string& id);

    unsigned long getHvdcConverterStationCount() const;

    const_range<HvdcConverterStation> getHvdcConverterStations() const;

    range<HvdcConverterStation> getHvdcConverterStations();

    const HvdcLine& getHvdcLine(const std::string& id) const;

    HvdcLine& getHvdcLine(const std::string& id);

    unsigned long getHvdcLineCount() const;

    const_range<HvdcLine> getHvdcLines() const;

    range<HvdcLine> getHvdcLines();

    const_range<Identifiable> getIdentifiables() const;

    range<Identifiable> getIdentifiables();

    const LccConverterStation& getLccConverterStation(const std::string& id) const;

    LccConverterStation& getLccConverterStation(const std::string& id);

    unsigned long getLccConverterStationCount() const;

    const_range<LccConverterStation> getLccConverterStations() const;

    range<LccConverterStation> getLccConverterStations();

    const Line& getLine(const std::string& id) const;

    Line& getLine(const std::string& id);

    unsigned long getLineCount() const;

    const_range<Line> getLines() const;

    range<Line> getLines();

    const Load& getLoad(const std::string& id) const;

    Load& getLoad(const std::string& id);

    unsigned long getLoadCount() const;

    const_range<Load> getLoads() const;

    range<Load> getLoads();

    const ShuntCompensator& getShuntCompensator(const std::string& id) const;

    ShuntCompensator& getShuntCompensator(const std::string& id);

    unsigned long getShuntCompensatorCount() const;

    const_range<ShuntCompensator> getShuntCompensators() const;

    range<ShuntCompensator> getShuntCompensators();

    const std::string& getSourceFormat() const;

    const_range<MultiVariantObject> getStatefulObjects() const;

    range<MultiVariantObject> getStatefulObjects();

    const StaticVarCompensator& getStaticVarCompensator(const std::string& id) const;

    StaticVarCompensator& getStaticVarCompensator(const std::string& id);

    unsigned long getStaticVarCompensatorCount() const;

    const_range<StaticVarCompensator> getStaticVarCompensators() const;

    range<StaticVarCompensator> getStaticVarCompensators();

    const Substation& getSubstation(const std::string& id) const;

    Substation& getSubstation(const std::string& id);

    unsigned long getSubstationCount() const;

    const_range<Substation> getSubstations() const;

    range<Substation> getSubstations();

    const Switch& getSwitch(const std::string& id) const;

    Switch& getSwitch(const std::string& id);

    unsigned long getSwitchCount() const;

    const_range<Switch> getSwitches() const;

    range<Switch> getSwitches();

    const ThreeWindingsTransformer& getThreeWindingsTransformer(const std::string& id) const;

    ThreeWindingsTransformer& getThreeWindingsTransformer(const std::string& id);

    unsigned long getThreeWindingsTransformerCount() const;

    const_range<ThreeWindingsTransformer> getThreeWindingsTransformers() const;

    range<ThreeWindingsTransformer> getThreeWindingsTransformers();

    const TwoWindingsTransformer& getTwoWindingsTransformer(const std::string& id) const;

    TwoWindingsTransformer& getTwoWindingsTransformer(const std::string& id);

    unsigned long getTwoWindingsTransformerCount() const;

    const_range<TwoWindingsTransformer> getTwoWindingsTransformers() const;

    range<TwoWindingsTransformer> getTwoWindingsTransformers();

    const VoltageLevel& getVoltageLevel(const std::string& id) const;

    VoltageLevel& getVoltageLevel(const std::string& id);

    unsigned long getVoltageLevelCount() const;

    const_range<VoltageLevel> getVoltageLevels() const;

    range<VoltageLevel> getVoltageLevels();

    const VscConverterStation& getVscConverterStation(const std::string& id) const;

    VscConverterStation& getVscConverterStation(const std::string& id);

    unsigned long getVscConverterStationCount() const;

    const_range<VscConverterStation> getVscConverterStations() const;

    range<VscConverterStation> getVscConverterStations();

    template <typename T, typename = typename std::enable_if<std::is_base_of<Identifiable, T>::value>::type>
    stdcxx::CReference<T> find(const std::string& id) const;

    template <typename T, typename = typename std::enable_if<std::is_base_of<Identifiable, T>::value>::type>
    stdcxx::Reference<T> find(const std::string& id);

    HvdcLineAdder newHvdcLine();

    LineAdder newLine();

    SubstationAdder newSubstation();

    TieLineAdder newTieLine();

    void remove(Identifiable& identifiable);

    Network& setCaseDate(const stdcxx::DateTime& caseDate);

    Network& setForecastDistance(int forecastDistance);

private: // Identifiable
    const std::string& getTypeDescription() const override;

private:
    template <typename T, typename = typename std::enable_if<std::is_base_of<Identifiable, T>::value>::type>
    unsigned long getObjectCount() const;

private:
    stdcxx::DateTime m_caseDate;

    int m_forecastDistance;

    std::string m_sourceFormat;

    NetworkIndex m_networkIndex;

    VariantManager m_variantManager;
};

}  // namespace iidm

}  // namespace powsybl

#include <powsybl/iidm/Network.hxx>

#endif  // POWSYBL_IIDM_NETWORK_HPP
