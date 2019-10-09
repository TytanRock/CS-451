pipeline {
	agent any


	stages {
		stage ('clean') {
			steps {
				sh 'rm -rf */bin/*'
				sh 'rm -rf */build/*'
			}
		}
		stage ('build 1') {
			steps {
				sh 'cd Ness1'
				sh 'sh compile.sh'
				sh 'cd ..'
			}
		}
	}
}
