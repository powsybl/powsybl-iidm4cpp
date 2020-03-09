/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_ENTSOE_ENTSOEEXPORT_HPP
#define POWSYBL_IIDM_EXTENSIONS_ENTSOE_ENTSOEEXPORT_HPP

#include <powsybl/stdcxx/export.hpp>

#ifdef EXT_ENTSOE_LIBRARY
#define EXT_ENTSOE_EXPORT DLL_EXPORT
#else
#define EXT_ENTSOE_EXPORT DLL_IMPORT
#endif

#endif // POWSYBL_IIDM_EXTENSIONS_ENTSOE_ENTSOEEXPORT_HPP
