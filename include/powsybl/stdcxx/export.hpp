/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_STDCXX_EXPORT_HPP
#define POWSYBL_STDCXX_EXPORT_HPP

#if defined _WIN32 || defined __CYGWIN__ || defined __MINGW32__
#ifdef __GNUC__
#define DLL_EXPORT __attribute__ ((dllexport))
#define DLL_IMPORT __attribute__ ((dllimport))
#else
#define DLL_EXPORT __declspec(dllexport)
#define DLL_IMPORT __declspec(dllimport)
#endif
#else
#if __GNUC__ >= 4
#define DLL_EXPORT __attribute__ ((visibility ("default")))
#define DLL_IMPORT  __attribute__ ((visibility ("hidden")))
#else
#define DLL_EXPORT
#define DLL_IMPORT
#endif
#endif

#endif  // POWSYBL_STDCXX_EXPORT_HPP
