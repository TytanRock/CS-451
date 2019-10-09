pipeline {
	agent : any
	stages {
		stage ('clean') {
			steps {
				sh 'rm */bin/*'
				sh 'rm */build/*'
			}
		}
		stage ('build 1') {
			steps {
				sh 'Ness1/compile.sh'
			}
		}
	}
}
