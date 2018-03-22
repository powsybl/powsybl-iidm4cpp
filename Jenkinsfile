node('build') {

    stage('Cloning git repository') {
        checkout scm
    }

    stage('Sonarqube') {
        sonar {
            useMaven = 'false'
        }
    }
}
