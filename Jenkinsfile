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
	}
}
