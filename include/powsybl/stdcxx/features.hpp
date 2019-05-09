/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_STDCXX_FEATURES_HPP
#define POWSYBL_STDCXX_FEATURES_HPP

#if defined __GNUC__ && defined __GNUC_MINOR__
#   define __GNUC_PREREQ(maj, min) ((__GNUC__ << 16) + __GNUC_MINOR__ >= ((maj) << 16) + (min))
#else
#   define __GNUC_PREREQ(maj, min) 0
#endif

#endif  // POWSYBL_STDCXX_FEATURES_HPP
