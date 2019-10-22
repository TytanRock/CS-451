pipeline {
	agent {
		dockerfile true
	}


	stages {
		stage ('clean') {
			steps {
				dir('Ness1') {
					sh 'make clean'
				}
			}
		}
		stage ('build 1') {
			steps {
				dir('Ness1') {
					sh 'make'
				}
			}
		}
		stage ('Test 1') {
			steps {
				dir('Ness1') {
					sh 'make bin/5ps_test'
					sh './bin/5ps_test'
				}
			}
		}
		stage ('Cover 1') {
			steps {
				dir('Ness1') {
					sh 'make gcov/5ps'
					dir('Ness1/gcov') {
						sh 'gcovr -r . --html > index.html'
					}
				}
			}
		}
	}

	post {
		always {
			junit 'Ness1/*.xml'
		publishHTML([allowMissing: false, alwaysLinkToLastBuild: false, keepAll: false, reportDir: 'Ness1/gcov', reportFiles: 'index.html', reportName: 'HTML Report', reportTitles: ''])
		}
	}
}
