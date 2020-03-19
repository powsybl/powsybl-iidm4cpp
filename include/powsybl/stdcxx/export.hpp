/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_STDCXX_EXPORT_HPP
#define POWSYBL_STDCXX_EXPORT_HPP

#if defined(_WIN32) && defined(IIDM_BUILD_DLL)
#define IIDM_DECLSPEC __declspec(dllexport)
#elif defined(_WIN32) && defined(IIDM_USE_DLL)
#define IIDM_DECLSPEC __declspec(dllimport)
#else
#define IIDM_DECLSPEC
#endif

#endif  // POWSYBL_STDCXX_EXPORT_HPP
