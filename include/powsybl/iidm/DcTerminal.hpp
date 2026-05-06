/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_DCTERMINAL_HPP
#define POWSYBL_IIDM_DCTERMINAL_HPP

#include <powsybl/iidm/MultiVariantObject.hpp>
#include <powsybl/iidm/TwoSides.hpp>
#include <powsybl/stdcxx/reference.hpp>

#include <vector>

namespace powsybl {

namespace iidm {

class DcConnectable;
class DcNode;
class Network;

class DcTerminal : public MultiVariantObject {

protected: //MultiVariantObject
    void allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) override;

    void deleteVariantArrayElement(unsigned long index) override;

    void extendVariantArraySize(unsigned long initVariantArraySize, unsigned long number, unsigned long sourceIndex) override;

    void reduceVariantArraySize(unsigned long number) override;

    friend class DcConnectable;
    friend class AcDcConverter;

public:
    explicit DcTerminal(DcNode& dcNode, const TwoSides& side, bool connected);
    ~DcTerminal() noexcept override = default;

    stdcxx::CReference<DcConnectable> getDcConnectable() const;
    stdcxx::Reference<DcConnectable> getDcConnectable();

    TwoSides getSide() const;

    const DcNode& getDcNode() const;
    DcNode& getDcNode();

    bool isConnected() const;
    DcTerminal& setConnected(bool connected);

    /**
     * Returns the active power (in MW) injected at the DC terminal.
     */
    double getP() const;
    /**
     * Set new active power (in MW) at this DC terminal.
     */
    DcTerminal& setP(double p);

    /**
     * Returns current (in A) at the DC terminal.
     */
    double getI() const;
    /**
     * Set new current (in A) at this DC terminal.
     */
    DcTerminal& setI(double i);

protected:
    const Network& getNetwork() const;
    DcTerminal& setDcConnectable(const stdcxx::Reference<DcConnectable>& dcConnectable);
    friend class DcConnectable;

private:
    DcNode& m_dcNode;

    TwoSides m_side;

    stdcxx::Reference<DcConnectable> m_dcConnectable;

    std::vector<bool> m_connected;
    std::vector<double> m_p;
    std::vector<double> m_i;

};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_DCTERMINAL_HPP