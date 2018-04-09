/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_LOGGERFACTORY_HPP
#define POWSYBL_IIDM_LOGGERFACTORY_HPP

#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <typeinfo>

#include <powsybl/logging/Logger.hpp>

namespace powsybl {

namespace logging {

class LoggerFactory {
public:
    /**
     * Return the unique instance of LoggerFactory
     *
     * @return the unique instance of LoggerFactory
     */
    static LoggerFactory& getInstance();

    /**
     * Return the logger corresponding to the name of the type T
     *
     * @tparam T the type to use to defined the logger's name
     *
     * @return the logger instance corresponding to the name of the type T
     */
    template <typename T>
    static Logger& getLogger();

    /**
     * Return the logger corresponding to the specified name
     *
     * @param name the name of the logger
     *
     * @return the logger corresponding to the specified name
     */
    static Logger& getLogger(const std::string& name);

public:
    /**
     * Add a logger corresponding to the name of type T
     *
     * @param logger the logger
     */
    template <typename T>
    void addLogger(std::unique_ptr<Logger>&& logger);

    /**
     * Add a logger corresponding to the specified name
     *
     * @param name the name corresponding to the logger
     * @param logger the logger
     */
    void addLogger(const std::string& name, std::unique_ptr<Logger>&& logger);

private:
    LoggerFactory() = default;

    ~LoggerFactory() = default;

    Logger& getLoggerByName(const std::string& name) const;

private:
    mutable std::recursive_mutex m_mutex;

    std::map<std::string, std::unique_ptr<Logger> > m_loggers;
};

}

}

#include <powsybl/logging/LoggerFactory.hxx>

#endif  // POWSYBL_IIDM_LOGGERFACTORY_HPP
