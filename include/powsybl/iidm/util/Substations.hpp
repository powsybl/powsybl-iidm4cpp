/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_UTIL_SUBSTATIONS_HPP
#define POWSYBL_IIDM_UTIL_SUBSTATIONS_HPP

namespace powsybl {

namespace iidm {

class Branch;
class HvdcConverterStation;
class Substation;
class ThreeWindingsTransformer;

namespace Substations {

void checkRemovability(const Substation& substation);

void checkRemovability(const Substation& substation, const Branch& branch);

void checkRemovability(const Substation& substation, const ThreeWindingsTransformer& twt);

void checkRemovability(const Substation& substation, const HvdcConverterStation& station);

}  // namespace Substations

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_UTIL_SUBSTATIONS_HPP
