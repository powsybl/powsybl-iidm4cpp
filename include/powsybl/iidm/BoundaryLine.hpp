/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_BOUNDARYLINE_HPP
#define POWSYBL_IIDM_BOUNDARYLINE_HPP

#include <powsybl/iidm/Boundary.hpp>
#include <powsybl/iidm/BoundaryLineGeneration.hpp>
#include <powsybl/iidm/FlowsLimitsHolder.hpp>
#include <powsybl/iidm/Injection.hpp>
#include <powsybl/iidm/OperationalLimitsGroups.hpp>
#include <powsybl/stdcxx/reference.hpp>

namespace powsybl {

namespace iidm {

class ActivePowerLimits;
class ActivePowerLimitsAdder;
class ApparentPowerLimits;
class ApparentPowerLimitsAdder;
class CurrentLimits;
class CurrentLimitsAdder;
class TieLine;

class BoundaryLine : public Injection, public FlowsLimitsHolder {
public:
    using Generation = boundary_line::Generation;

public:  // Identifiable
    const IdentifiableType& getType() const override;

public:
    BoundaryLine(VariantManagerHolder& network, const std::string& id, const std::string& name, bool fictitious,
                 double p0, double q0, double r, double x, double g, double b, const std::string& pairingKey,
                 std::unique_ptr<Generation>&& generation);

    ~BoundaryLine() noexcept override = default;

    double getB() const;

    const Boundary& getBoundary() const;

    Boundary& getBoundary();

    stdcxx::CReference<TieLine> getTieLine() const;
    stdcxx::Reference<TieLine> getTieLine();

    BoundaryLine& setTieLine(TieLine& tieLine);

    void remove() override;
    void removeTieLine();

    double getG() const;

    stdcxx::CReference<Generation> getGeneration() const;

    stdcxx::Reference<Generation> getGeneration();

    bool isPaired() const;

    double getP0() const;

    double getQ0() const;

    double getR() const;

    const std::string& getPairingKey() const;

    double getX() const;

    BoundaryLine& setB(double b);

    BoundaryLine& setG(double g);

    BoundaryLine& setP0(double p0);

    BoundaryLine& setQ0(double q0);

    BoundaryLine& setR(double r);

    BoundaryLine& setX(double x);

    /**
     * Set pairing key only if this boundary line is not paired,
     * throw exception if already paired.
     */
    BoundaryLine& setPairingKey(const std::string& pairingKey);

// FlowsLimitsHolder
    stdcxx::const_range<OperationalLimitsGroup> getOperationalLimitsGroups() const override;
    stdcxx::range<OperationalLimitsGroup> getOperationalLimitsGroups() override;
    stdcxx::optional<std::string> getSelectedOperationalLimitsGroupId() const override;
    const std::list<std::string>& getAllSelectedOperationalLimitsGroupIds() const override;
    stdcxx::CReference<OperationalLimitsGroup> getOperationalLimitsGroup(const std::string& id) const override;
    stdcxx::Reference<OperationalLimitsGroup> getOperationalLimitsGroup(const std::string& id) override;
    stdcxx::CReference<OperationalLimitsGroup> getSelectedOperationalLimitsGroup() const override;
    stdcxx::Reference<OperationalLimitsGroup> getSelectedOperationalLimitsGroup() override;
    stdcxx::const_range<OperationalLimitsGroup> getAllSelectedOperationalLimitsGroups() const override;
    stdcxx::range<OperationalLimitsGroup> getAllSelectedOperationalLimitsGroups() override;
    OperationalLimitsGroup& newOperationalLimitsGroup(const std::string& id) override;
    void setSelectedOperationalLimitsGroup(const std::string& id) override;
    void addSelectedOperationalLimitsGroups(const std::list<std::string>& ids) override;
    void removeOperationalLimitsGroup(const std::string& id) override;
    void cancelSelectedOperationalLimitsGroup() override;
    void deselectOperationalLimitsGroups(const std::list<std::string>& ids) override;
    /**
     * Get the OperationalLimitsGroup selected or create a new one if it does not exist and set it as selected.
     */
    virtual OperationalLimitsGroup& getOrCreateSelectedOperationalLimitsGroup() override;
    /**
     * Get the OperationalLimitsGroup corresponding to the given id or create a new one if it does not exist and set it as selected.
     */
    virtual OperationalLimitsGroup& getOrCreateSelectedOperationalLimitsGroup(const std::string& id) override;

    /** \deprecated
    * Use OperationalLimitsGroup::newActivePowerLimits() instead.
    */
    ActivePowerLimitsAdder newActivePowerLimits() override;
    /** \deprecated
    * Use OperationalLimitsGroup::newApparentPowerLimits() instead.
    */
    ApparentPowerLimitsAdder newApparentPowerLimits() override;
    /** \deprecated
    * Use OperationalLimitsGroup::newCurrentLimits() instead.
    */
    CurrentLimitsAdder newCurrentLimits() override;

    void applySolvedValues();
    void setGenerationTargetPToP();
    void setGenerationTargetQToQ();
    void setGenerationTargetVToV();

protected: // MultiVariantObject
    void allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) override;

    void extendVariantArraySize(unsigned long initVariantArraySize, unsigned long number, unsigned long sourceIndex) override;

    void reduceVariantArraySize(unsigned long number) override;

private: // Identifiable
    const std::string& getTypeDescription() const override;

private:
    double m_b;

    double m_g;

    double m_r;

    double m_x;

    std::vector<double> m_p0;

    std::vector<double> m_q0;

    std::string m_pairingKey;

    std::unique_ptr<Generation> m_generation;

    std::unique_ptr<Boundary> m_boundary;

    stdcxx::Reference<TieLine> m_tieLine;

    OperationalLimitsGroups m_operationalLimitsGroups;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_BOUNDARYLINE_HPP
