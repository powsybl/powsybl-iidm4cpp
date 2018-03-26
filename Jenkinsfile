/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

def maintainers = "mathieu.bague@rte-france.com"

node('convergence-centos7') {

    try {
        stage('Cloning git repository') {
            checkout scm
        }

        stage('Building') {
            sh 'rm -rf build'

            dir('build') {
                sh """
                cmake -DCMAKE_BUILD_TYPE=Debug -DBOOST_ROOT=/users/gsr/boost-1.59.0/4.8.2 -DGTEST_ROOT=/users/gsr/googletest-master/4.8.2 -DCODE_COVERAGE=TRUE ..
                make -j8
                """
            }
        }

        stage('Code-coverage') {
            // Run tests and compute code coverage results
            dir('build') {
                sh """
                make code-coverage

                # Clean non relevant files
                rm -f coverage/reports/#usr#*
                rm -f coverage/reports/#users#gsr#*
                """
            }

            // Stash
            stash includes: 'build/coverage/reports/*.gcov', name: 'code-coverage'
        }
    } catch (Exception e) {
        notify {
            to = maintainers
            errorMsg = e.toString()
        }
        throw e
    } finally {
        step([$class: 'WsCleanup'])
    }
}

node('build') {

    try {
        stage('Sonarqube') {
            checkout scm

            // Clean old code coverage results
            sh 'rm -rf build'

            // Unstash new code coverage results
            unstash 'code-coverage'

            // Run sonar analysis
            sonar {
                useMaven = 'false'
            }
        }
    } catch (Exception e) {
        notify {
            to = maintainers
            errorMsg = e.toString()
        }
        throw e
    } finally {
        step([$class: 'WsCleanup'])
    }
}
