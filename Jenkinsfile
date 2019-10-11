pipeline {
	agent {
		docker {
			image 'gcc:latest'
		}
	}


	stages {
		stage ('clean') {
			steps {
				dir('NessRenoSmith1') {
					sh 'make clean'
				}
			}
		}
		stage ('build 1') {
			steps {
				dir('NessRenoSmith1') {
					sh 'make'
				}
			}
		}
		stage ('Test 1') {
			steps {
				dir('NessRenoSmith1') {
					sh 'make bin/5ps_test'
					sh './bin/5ps_test'
				}
			}
		}
	}

	post {
		always {
			junit 'NessRenoSmith1/*.xml'
		}
	}
}
