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

#

#
# @version Last updated 2025-01-26
# Debian Bookworm (Debian 12)
# Image from:
#
#     https://hub.docker.com/_/debian
#
FROM debian:12.9-slim

#
# Docker's builtin TARGETARCH build arg:
#
#     https://docs.docker.com/engine/reference/builder/?_gl=1*13mno0d*_ga*NjYxNDI5MzM5LjE2OTQxMDIzNzI.*_ga_XJWPQMJYHQ*MTY5NDQ1MzA1OS4yLjEuMTY5NDQ1MzI4Ny4yOC4wLjA.#automatic-platform-args-in-the-global-scope
#
ARG TARGETARCH

USER root

ENV DEBIAN_FRONTEND=noninteractive
ENV TZ=UTC/UTC

#
# Packages for hashtable:
#
#     ca-certificates
#         Latest certificate authorities.
#     gcc
#         C compiler.
#     gdb
#         C debugger.
#     libpcre2-dev
#         UTF-8 character regular expressions.
#     libutf8proc-dev
#         UTF-8 character reading and categorizing.
#     libutf8proc2
#         UTF-8 character reading and categorizing.
#     locales [required] [dynamic]
#         Required both for building C code, and for runtime reading
#         and writing of UTF-8-encoded characters.
#     make
#         Traditional make.  Required for building things from Makefiles.
#     utf8lex
#         UTF-8 string lexical analysis and reading.
#
RUN apt-get update --yes \
    && apt-get install --no-install-recommends --yes \
       ca-certificates \
       gcc \
       gdb \
       libpcre2-dev \
       libutf8proc-dev \
       libutf8proc2 \
       locales \
       make \
    && apt-get clean

ENV LC_CTYPE=C.utf8

#
# TODO TEMPORARY create proper packages from utf8lex repo instead of these
# TODO manually downloaded binaries.
#
COPY ./include/utf8lex.h /usr/include/
COPY ./lib/libutf8lex.so.1.0.0.linux.${TARGETARCH} /usr/lib/libutf8lex.so.1.0.0
RUN ln -s /usr/lib/libutf8lex.so.1.0.0 /usr/lib/libutf8lex.so.1 \
    && ln -s /usr/lib/libutf8lex.so.1.0.0 /usr/lib/libutf8lex.so

#
# User hashtable
#
# Install a non-root user, to mitigate intrusions and mistakes
# at runtime.
#
# 32 digit random number for hashtable user's password.
#
RUN mkdir /home/hashtable \
    && useradd \
           --home-dir /home/hashtable \
           --password `od --read-bytes 32 --format u --address-radix none /dev/urandom | tr --delete ' \n'` \
           hashtable \
    && chown -R hashtable:hashtable /home/hashtable

#
# hashtable working directory
#
RUN mkdir -p /hashtable \
    && chown -R hashtable:hashtable /hashtable \
    && chmod ug+rwx,o-rwx /hashtable

USER hashtable
WORKDIR /hashtable

#
# No default entrypoint.
#
CMD [""]
ENTRYPOINT [""]
