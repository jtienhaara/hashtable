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

HASHTABLE_VERSION ?= 1.0.0
HASHTABLE_MAJOR ?= $(word 1, $(subst ., ,$(HASHTABLE_VERSION)))

#
# Make sure the path we use to load shared libraries is absolute, not relative:
#
HASHTABLE_LIBRARY_DIR = $(subst $(PWD)//,/,$(PWD)/$(BUILD_DIR))
HASHTABLE_LIBRARY_PATH = $(if $(LD_LIBRARY_PATH),$(HASHTABLE_LIBRARY_DIR):$(LD_LIBRARY_PATH),$(HASHTABLE_LIBRARY_DIR))

#
# Parameters for gcc compiling .c into .o files, .o files into exes, etc:
#
CC = gcc
CFLAGS = -Werror -fPIC -O2
# CFLAGS = -Werror -fPIC -O2 --debug

LD = gcc
LDFLAGS_LIBRARY = -shared

BUILD_DIR ?= ../build

SOURCE_FILES ?= \
	hashtable.c

OBJECT_FILES = \
	$(patsubst %.c,$(BUILD_DIR)/%.o,$(SOURCE_FILES))

LIBRARY = \
	$(BUILD_DIR)/libhashtable.so.$(HASHTABLE_VERSION)

LC_CTYPE = en_US.UTF-8

.PHONY: all
all: build

# Rule for $(OBJECT_FILES):
$(BUILD_DIR)/%.o: %.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Rule for .so library files:
$(LIBRARY): $(OBJECT_FILES)
	$(LD) $(LDFLAGS_LIBRARY)  \
	    -Wl,-soname,libhashtable.so.$(HASHTABLE_MAJOR) \
	    $(OBJECT_FILES) \
	    -o $(BUILD_DIR)/libhashtable.so.$(HASHTABLE_VERSION)
	chmod a+x $(BUILD_DIR)/libhashtable.so.$(HASHTABLE_VERSION)
	cd $(BUILD_DIR) \
	    && if test "$(HASHTABLE_MAJOR)" != "$(HASHTABLE_VERSION)" ; \
                then \
	            ln -s -f libhashtable.so.$(HASHTABLE_VERSION) \
	                libhashtable.so.$(HASHTABLE_MAJOR) ; \
	        fi \
	    && ln -s -f libhashtable.so.$(HASHTABLE_VERSION) \
	        libhashtable.so

.PHONY: build
build: $(LIBRARY)

.PHONY: clean
clean:
	rm -f core
	rm -f $(OBJECT_FILES)
	rm -f \
	    $(BUILD_DIR)/libhashtable.so.$(HASHTABLE_VERSION) \
	    $(BUILD_DIR)/libhashtable.so.$(HASHTABLE_MAJOR) \
	    $(BUILD_DIR)/libhashtable.so \
	    $(BUILD_DIR)/hashtable.$(HASHTABLE_VERSION) \
	    $(BUILD_DIR)/hashtable.$(HASHTABLE_MAJOR)
