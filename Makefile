#
# hashtable
# Copyright © 2025-2025 Johann Tienhaara
# All rights reserved
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

.PHONY: all
all: build test

.PHONY: build-container
build-container:
	docker build . \
	    --file builder.Dockerfile \
	    --tag hashtable:latest

.PHONY: container
container: build-container
	docker run \
	    --rm \
	    --volume `pwd`:/hashtable:rw \
	    hashtable:latest \
	    bash -c 'make clean && make all'

.PHONY: container-debug
container-debug: build-container
	docker run \
	    --rm \
	    -i --tty \
	    --volume `pwd`:/hashtable:rw \
	    hashtable:latest \
	    bash -c 'make clean && make build && make all && echo "***** No core dump, no debug *****" || make debug'

.PHONY: build
build:
	cd src \
	    && make build

clean:
	cd src \
	    && make clean
	cd tests/unit \
	    && make clean

.PHONY: test
test: unit_tests

.PHONY: unit_tests
unit_tests:
	cd tests/unit \
	    && make build \
	    && make run
