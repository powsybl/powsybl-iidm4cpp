/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_DCTERMINAL_HPP
#define POWSYBL_IIDM_DCTERMINAL_HPP

#include <powsybl/iidm/DcTerminalSet.hpp>
#include <powsybl/iidm/DcTerminalTopologyTraverser.hpp>
#include <powsybl/iidm/MultiVariantObject.hpp>
#include <powsybl/iidm/TerminalNumber.hpp>
#include <powsybl/iidm/TwoSides.hpp>
#include <powsybl/math/TraversalType.hpp>
#include <powsybl/stdcxx/reference.hpp>

#include <vector>

namespace powsybl {

namespace iidm {

class DcBus;
class DcConnectable;
class DcNode;
class Network;

class DcTerminal : public MultiVariantObject {

public:
    using DcTopologyTraverser = dc_terminal::TopologyTraverser;

protected: //MultiVariantObject
    void allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) override;

    void deleteVariantArrayElement(unsigned long index) override;

    void extendVariantArraySize(unsigned long initVariantArraySize, unsigned long number, unsigned long sourceIndex) override;

    void reduceVariantArraySize(unsigned long number) override;

    friend class DcConnectable;
    friend class AcDcConverter;

public:
    template <typename T, typename = typename std::enable_if<std::is_base_of<DcConnectable, T>::value>::type>
    static bool isInstanceOf(const DcTerminal& dcTerminal);

    template <typename T, typename = typename std::enable_if<std::is_base_of<DcConnectable, T>::value>::type>
    static const T& map(const DcTerminal& dcTerminal);

public:
    /**
     * DcTerminal constructor without side nor terminal number
     */
    explicit DcTerminal(DcNode& dcNode, bool connected);
    /**
     * DcTerminal constructor for a side of a DcConnectable
     */
    explicit DcTerminal(DcNode& dcNode, const TwoSides& side, bool connected);
    /**
     * DcTerminal constructor for a terminal number of an AcDcConverter
     */
    explicit DcTerminal(DcNode& dcNode, const TerminalNumber& terminalNumber, bool connected);

    ~DcTerminal() noexcept override = default;

    stdcxx::CReference<DcConnectable> getDcConnectable() const;
    stdcxx::Reference<DcConnectable> getDcConnectable();

    TwoSides getSide() const;
    TerminalNumber getTerminalNumber() const;

    const DcNode& getDcNode() const;
    DcNode& getDcNode();

    bool isConnected() const;
    DcTerminal& setConnected(bool connected);

    stdcxx::CReference<DcBus> getDcBus() const;
    stdcxx::Reference<DcBus> getDcBus();

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

    /**
     * Traverse the full network DC Topology graph usig a "Depth-first" TraversalType
     */
    bool traverse(DcTopologyTraverser& traverser);
    /**
     * Traverse the full network DC Topology graph.
     */
    bool traverse(DcTopologyTraverser& traverser, math::TraversalType traversalType);

    /**
     * Traverse the full network DC Topology graph, keeping track on the traversed DC Terminals.
     */
    bool traverse(DcTopologyTraverser& traverser, DcTerminalSet& traversedDcTerminals, math::TraversalType traversalType);

    /**
     * Connects this DC Terminal for the working Variant
     * 
     * @return true if the terminal has been connected, false if already connected
     */
    bool connect();
    /**
     * Disconnects this DC Terminal for the working Variant
     * 
     * @return true if the terminal has been disconnected, false if already disconnected
     */
    bool disconnect();

protected:
    const Network& getNetwork() const;
    const Network& getParentNetwork() const;
    Network& getParentNetwork();

    DcTerminal& setDcConnectable(const stdcxx::Reference<DcConnectable>& dcConnectable);
    friend class DcConnectable;

private:
    DcNode& m_dcNode;

    TwoSides m_side = TwoSides::UNDEFINED;
    TerminalNumber m_terminalNumber = TerminalNumber::UNDEFINED;

    stdcxx::Reference<DcConnectable> m_dcConnectable;

    std::vector<bool> m_connected;
    std::vector<double> m_p;
    std::vector<double> m_i;

};

std::ostream& operator<<(std::ostream& stream, const DcTerminal& dcTerminal);

}  // namespace iidm

}  // namespace powsybl

#include <powsybl/iidm/DcTerminal.hxx>

#endif  // POWSYBL_IIDM_DCTERMINAL_HPP