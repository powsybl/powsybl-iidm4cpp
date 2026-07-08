/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_DCTOPOLOGYVISITABLE_HPP
#define POWSYBL_IIDM_DCTOPOLOGYVISITABLE_HPP

#include <type_traits>

#include <powsybl/stdcxx/range.hpp>

namespace powsybl {

namespace iidm {

class DcConnectable;
class DcGround;
class DcLine;
class DcTerminal;
class DcTopologyVisitor;
class LineCommutatedConverter;
class VoltageSourceConverter;

class DcTopologyVisitable {

public :
    DcTopologyVisitable() = default;

    DcTopologyVisitable(const DcTopologyVisitable&) = default;

    DcTopologyVisitable(DcTopologyVisitable&&) noexcept = default;

    DcTopologyVisitable& operator=(const DcTopologyVisitable&) = default;

    DcTopologyVisitable& operator=(DcTopologyVisitable&&) noexcept = default;

    virtual ~DcTopologyVisitable() noexcept = default;

public:
    virtual unsigned long getDcTerminalCount() const = 0;

    virtual stdcxx::const_range<DcTerminal> getDcTerminals() const = 0;

    virtual stdcxx::range<DcTerminal> getDcTerminals() = 0;

    virtual unsigned long getConnectedDcTerminalCount() const = 0;

    virtual stdcxx::const_range<DcTerminal> getConnectedDcTerminals() const = 0;

    virtual stdcxx::range<DcTerminal> getConnectedDcTerminals() = 0;


    virtual unsigned long getDcGroundCount() const;
    virtual stdcxx::const_range<DcGround> getDcGrounds() const;
    virtual stdcxx::range<DcGround> getDcGrounds();

    virtual unsigned long getDcLineCount() const;
    virtual stdcxx::const_range<DcLine> getDcLines() const;
    virtual stdcxx::range<DcLine> getDcLines();

    virtual unsigned long getLineCommutatedConverterCount() const;
    virtual stdcxx::const_range<LineCommutatedConverter> getLineCommutatedConverters() const;
    virtual stdcxx::range<LineCommutatedConverter> getLineCommutatedConverters();

    virtual unsigned long getVoltageSourceConverterCount() const;
    virtual stdcxx::const_range<VoltageSourceConverter> getVoltageSourceConverters() const;
    virtual stdcxx::range<VoltageSourceConverter> getVoltageSourceConverters();

    /**
     * Visit Dc equipments connected.
     */
    virtual void visitConnectedEquipments(DcTopologyVisitor& visitor) = 0;

    /**
     * Visit Dc equipments connected or connectable.
     */
    virtual void visitConnectedOrConnectableEquipments(DcTopologyVisitor& visitor) = 0;


protected:
    template <typename T = DcConnectable, typename = typename std::enable_if<std::is_base_of<DcConnectable, T>::value>::type>
    unsigned long getDcConnectableCount() const;

    template <typename T = DcConnectable, typename = typename std::enable_if<std::is_base_of<DcConnectable, T>::value>::type>
    stdcxx::const_range<T> getDcConnectables() const;

    template <typename T = DcConnectable, typename = typename std::enable_if<std::is_base_of<DcConnectable, T>::value>::type>
    stdcxx::range<T> getDcConnectables();

};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_DCTOPOLOGYVISITABLE_HPP
