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

public: // VariantManagerHolder
    unsigned long getVariantIndex() const override;

    const VariantManager& getVariantManager() const override;

    VariantManager& getVariantManager() override;

public:
    Network(const std::string& id, const std::string& sourceFormat);

    Network(Network&&) = default;

    ~Network() noexcept override = default;

    template <typename T> NetworkIndex::iterator<T> begin() {
        return m_networkIndex.begin<T>();
    }

    template <typename T> NetworkIndex::const_iterator<T> cbegin() const {
        return m_networkIndex.cbegin<T>();
    }

    template <typename T> NetworkIndex::const_iterator<T> cend() const {
        return m_networkIndex.cend<T>();
    }

    template <typename T> T& checkAndAdd(std::unique_ptr<T>&& identifiable) {
        return m_networkIndex.checkAndAdd(std::move(identifiable));
    }

    template <typename T> NetworkIndex::iterator<T> end() {
        return m_networkIndex.end<T>();
    }

    template <typename T, typename = typename std::enable_if<std::is_base_of<Identifiable, T>::value>::type>
    const T& get(const std::string& id) const;

    template <typename T, typename = typename std::enable_if<std::is_base_of<Identifiable, T>::value>::type>
    T& get(const std::string& id);

    const Battery& getBattery(const std::string& id) const;

    Battery& getBattery(const std::string& id);

    unsigned long getBatteryCount() const;

    const Branch& getBranch(const std::string& id) const;

    Branch& getBranch(const std::string& id);

    unsigned long getBranchCount() const;

    const BusbarSection& getBusbarSection(const std::string& id) const;

    BusbarSection& getBusbarSection(const std::string& id);

    unsigned long getBusbarSectionCount() const;

    const stdcxx::DateTime& getCaseDate() const;

    unsigned long getCountryCount() const;

    const DanglingLine& getDanglingLine(const std::string& id) const;

    DanglingLine& getDanglingLine(const std::string& id);

    unsigned long getDanglingLineCount() const;

    int getForecastDistance() const;

    const Generator& getGenerator(const std::string& id) const;

    Generator& getGenerator(const std::string& id);

    unsigned long getGeneratorCount() const;

    const HvdcConverterStation& getHvdcConverterStation(const std::string& id) const;

    HvdcConverterStation& getHvdcConverterStation(const std::string& id);

    unsigned long getHvdcConverterStationCount() const;

    const HvdcLine& getHvdcLine(const std::string& id) const;

    HvdcLine& getHvdcLine(const std::string& id);

    unsigned long getHvdcLineCount() const;

    const LccConverterStation& getLccConverterStation(const std::string& id) const;

    LccConverterStation& getLccConverterStation(const std::string& id);

    unsigned long getLccConverterStationCount() const;

    const Line& getLine(const std::string& id) const;

    Line& getLine(const std::string& id);

    unsigned long getLineCount() const;

    const Load& getLoad(const std::string& id) const;

    Load& getLoad(const std::string& id);

    unsigned long getLoadCount() const;

    template <typename T, typename = typename std::enable_if<std::is_base_of<Identifiable, T>::value>::type>
    unsigned long getObjectCount() const;

    const ShuntCompensator& getShuntCompensator(const std::string& id) const;

    ShuntCompensator& getShuntCompensator(const std::string& id);

    unsigned long getShuntCompensatorCount() const;

    const std::string& getSourceFormat() const;

    const StaticVarCompensator& getStaticVarCompensator(const std::string& id) const;

    StaticVarCompensator& getStaticVarCompensator(const std::string& id);

    unsigned long getStaticVarCompensatorCount() const;

    const Substation& getSubstation(const std::string& id) const;

    Substation& getSubstation(const std::string& id);

    unsigned long getSubstationCount() const;

    const Switch& getSwitch(const std::string& id) const;

    Switch& getSwitch(const std::string& id);

    unsigned long getSwitchCount() const;

    const ThreeWindingsTransformer& getThreeWindingsTransformer(const std::string& id) const;

    ThreeWindingsTransformer& getThreeWindingsTransformer(const std::string& id);

    unsigned long getThreeWindingsTransformerCount() const;

    const TwoWindingsTransformer& getTwoWindingsTransformer(const std::string& id) const;

    TwoWindingsTransformer& getTwoWindingsTransformer(const std::string& id);

    unsigned long getTwoWindingsTransformerCount() const;

    const VoltageLevel& getVoltageLevel(const std::string& id) const;

    VoltageLevel& getVoltageLevel(const std::string& id);

    unsigned long getVoltageLevelCount() const;

    const VscConverterStation& getVscConverterStation(const std::string& id) const;

    VscConverterStation& getVscConverterStation(const std::string& id);

    unsigned long getVscConverterStationCount() const;

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
