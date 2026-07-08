/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/DcTopologyVisitable.hpp>

#include <powsybl/iidm/DcConnectable.hpp>
#include <powsybl/iidm/DcGround.hpp>
#include <powsybl/iidm/DcLine.hpp>
#include <powsybl/iidm/DcTerminal.hpp>
#include <powsybl/iidm/DcTopologyVisitor.hpp>
#include <powsybl/iidm/LineCommutatedConverter.hpp>
#include <powsybl/iidm/VoltageSourceConverter.hpp>

#include <boost/range/adaptor/filtered.hpp>
#include <boost/range/adaptor/transformed.hpp>

namespace powsybl {

namespace iidm {

unsigned long DcTopologyVisitable::getDcGroundCount() const {
    return getDcConnectableCount<DcGround>();
};
stdcxx::const_range<DcGround> DcTopologyVisitable::getDcGrounds() const {
    return getDcConnectables<DcGround>();
};
stdcxx::range<DcGround> DcTopologyVisitable::getDcGrounds() {
    return getDcConnectables<DcGround>();
};

unsigned long DcTopologyVisitable::getDcLineCount() const {
    return getDcConnectableCount<DcLine>();
};
stdcxx::const_range<DcLine> DcTopologyVisitable::getDcLines() const {
    return getDcConnectables<DcLine>();
};
stdcxx::range<DcLine> DcTopologyVisitable::getDcLines() {
    return getDcConnectables<DcLine>();
};

unsigned long DcTopologyVisitable::getLineCommutatedConverterCount() const {
    return getDcConnectableCount<LineCommutatedConverter>();
};
stdcxx::const_range<LineCommutatedConverter> DcTopologyVisitable::getLineCommutatedConverters() const {
    return getDcConnectables<LineCommutatedConverter>();
};
stdcxx::range<LineCommutatedConverter> DcTopologyVisitable::getLineCommutatedConverters() {
    return getDcConnectables<LineCommutatedConverter>();
};

unsigned long DcTopologyVisitable::getVoltageSourceConverterCount() const {
    return getDcConnectableCount<VoltageSourceConverter>();
};
stdcxx::const_range<VoltageSourceConverter> DcTopologyVisitable::getVoltageSourceConverters() const {
    return getDcConnectables<VoltageSourceConverter>();
};
stdcxx::range<VoltageSourceConverter> DcTopologyVisitable::getVoltageSourceConverters() {
    return getDcConnectables<VoltageSourceConverter>();
};

template <typename T, typename>
unsigned long DcTopologyVisitable::getDcConnectableCount() const {
    return boost::size(getDcConnectables<T>());
}

template <typename T, typename>
stdcxx::const_range<T> DcTopologyVisitable::getDcConnectables() const {
    return getConnectedDcTerminals() | boost::adaptors::filtered(DcTerminal::isInstanceOf<T>) | boost::adaptors::transformed(DcTerminal::map<const T>);
}

template <typename T, typename>
stdcxx::range<T> DcTopologyVisitable::getDcConnectables() {
    return getConnectedDcTerminals() | boost::adaptors::filtered(DcTerminal::isInstanceOf<T>) | boost::adaptors::transformed(DcTerminal::map<T>);
}



}  // namespace iidm

}  // namespace powsybl
