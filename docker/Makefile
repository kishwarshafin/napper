# Definitions
git_commit ?= $(shell git log --pretty=oneline -n 1 | cut -f1 -d " ")
name = tpesout/napper
tag = ${git_commit}

build:
	# do the docker build
	docker build --build-arg git_commit=${tag} -t ${name}:${tag} .
	docker tag ${name}:${tag} ${name}:latest

push: build
	# Requires ~/.dockercfg
	docker push ${name}:${tag}
	docker push ${name}:latest
