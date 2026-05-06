/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_AREA_HPP
#define POWSYBL_IIDM_AREA_HPP

#include <powsybl/iidm/Identifiable.hpp>

#include <powsybl/iidm/AreaBoundaryAdder.hpp>
#include <powsybl/iidm/NetworkRef.hpp>

#include <powsybl/stdcxx/Predicate.hpp>
#include <powsybl/stdcxx/reference.hpp>

namespace powsybl {

namespace iidm {

class AreaBoundary;
class Boundary;
class Terminal;
class VoltageLevel;

class Area : public Identifiable {

public:  // Identifiable
    virtual const Network& getNetwork() const override;

    virtual Network& getNetwork() override;

    virtual const Network& getParentNetwork() const override;

    virtual Network& getParentNetwork() override;

    virtual const IdentifiableType& getType() const override;

public: // MultiVariantObject
    virtual void allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) override;

    virtual void extendVariantArraySize(unsigned long initVariantArraySize, unsigned long number, unsigned long sourceIndex) override;

    virtual void reduceVariantArraySize(unsigned long number) override;

private:
    Area(Network& network, const std::string& id, const std::string& name, bool fictitious, const std::string& areaType, double interchangeTarget);
    Area(Network& rootNetwork, Network& subNetwork, const std::string& id, const std::string& name, bool fictitious, const std::string& areaType, double interchangeTarget);

    friend class AreaAdder;

public:
    ~Area() noexcept override = default;

private: // Identifiable
    const std::string& getTypeDescription() const override;

private:
    void setNetworkRef(Network& network);
    friend class NetworkIndex;

    stdcxx::Reference<Network>& getSubNetworkRef();
    void setSubNetworkRef(const stdcxx::Reference<Network>& subNetwork);

public:
    const std::string& getAreaType() const;

    stdcxx::const_range<VoltageLevel> getVoltageLevels() const;
    stdcxx::range<VoltageLevel> getVoltageLevels();

    double getInterchangeTarget() const;
    Area& setInterchangeTarget(double interchangeTarget);

    double getAcInterchange() const;
    double getDcInterchange() const;
    double getInterchange() const;
    double getInterchange(const stdcxx::Predicate<AreaBoundary>& predicate) const;

    Area& addVoltageLevel(VoltageLevel& voltageLevel);
    Area& removeVoltageLevel(VoltageLevel& voltageLevel);

    AreaBoundaryAdder newAreaBoundary();
private:
    Area& addAreaBoundary(const std::shared_ptr<AreaBoundary>& areaBoundary);
    void checkBoundaryNetwork(const Network& network, const std::string& boundaryTypeAndId);
    friend class AreaBoundaryAdder;

public:
    Area& removeAreaBoundary(const Terminal& terminal);
    Area& removeAreaBoundary(const Boundary& boundary);

    std::shared_ptr<AreaBoundary> getAreaBoundary(const Terminal& terminal);
    stdcxx::CReference<AreaBoundary> getAreaBoundary(const Terminal& terminal) const;
    std::shared_ptr<AreaBoundary> getAreaBoundary(const Boundary& boundary);
    stdcxx::CReference<AreaBoundary> getAreaBoundary(const Boundary& boundary) const;

    std::vector<std::shared_ptr<AreaBoundary>>& getAreaBoundaries();
    stdcxx::const_range<AreaBoundary> getAreaBoundaries() const;

    void remove();

private:
    NetworkRef m_rootNetwork;
    stdcxx::Reference<Network> m_subNetwork;

    std::string m_areaType;

    std::vector<std::shared_ptr<AreaBoundary>> m_areaBoundaries;

    std::vector<std::reference_wrapper<VoltageLevel>> m_voltageLevels;

    std::vector<double> m_interchangeTarget;

};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_AREA_HPP
