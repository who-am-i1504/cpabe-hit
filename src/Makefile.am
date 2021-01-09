#
# Licensed to the Apache Software Foundation (ASF) under one
# or more contributor license agreements. See the NOTICE file
# distributed with this work for additional information
# regarding copyright ownership. The ASF licenses this file
# to you under the Apache License, Version 2.0 (the
# "License"); you may not use this file except in compliance
# with the License. You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing,
# software distributed under the License is distributed on an
# "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
# KIND, either express or implied. See the License for the
# specific language governing permissions and limitations
# under the License.
#
AUTOMAKE_OPTIONS = foreign subdir-objects serial-tests nostdinc

# INCLUDES= \
#          -I. \
#          -I../include \
#          -I/usr/include/glib-2.0 \
#          -I/usr/lib/x86_64-linux-gnu/glib-2.0/include \
#          -I/usr/include/pbc \
#          -I/usr/local/include/pbc \
#          -I/usr/local/include/thrift


BUILT_SOURCES = \
	gen-c_glib/aa_aa_types.h \
	gen-c_glib/aa_authority.h \
	gen-c_glib/ca_ca_types.h \
	gen-c_glib/ca_center.h \
	gen-c_glib/user_user_types.h \
	gen-c_glib/user_user.h \
	gen-c_glib/cloud_cloud_types.h \
	gen-c_glib/cloud_cloud.h \
	gen-c_glib/policy_policy_types.h \
	gen-c_glib/policy_policy.h



AM_CFLAGS = -g -Wall -Wextra -pedantic \
            -I/usr/include/pbc \
            -I/usr/local/include/pbc \
            -I/usr/include/glib-2.0 \
            -I/usr/lib/x86_64-linux-gnu/glib-2.0/include \
            -I/usr/include/openssl \
			-I/usr/local/include/thrift \
            -I../include \
			-I.

AM_CPPFLAGS = -I/usr/local/include/ -Igen-c_glib
AM_LDFLAGS = -lglib-2.0 \
             -lgmp \
             -lcrypto \
             -lpbc \
             -lm

noinst_LTLIBRARIES = \
	libtutorialgencglib.la

nodist_libtutorialgencglib_la_SOURCES = \
	gen-c_glib/aa_aa_types.h \
	gen-c_glib/aa_aa_types.c \
	gen-c_glib/aa_authority.h \
	gen-c_glib/aa_authority.c \
	gen-c_glib/ca_ca_types.h \
	gen-c_glib/ca_ca_types.c \
	gen-c_glib/ca_center.h \
	gen-c_glib/ca_center.c \
	gen-c_glib/user_user_types.h \
	gen-c_glib/user_user_types.c \
	gen-c_glib/user_user.h \
	gen-c_glib/user_user.c \
	gen-c_glib/cloud_cloud_types.h \
	gen-c_glib/cloud_cloud_types.c \
	gen-c_glib/cloud_cloud.h \
	gen-c_glib/cloud_cloud.c \
	gen-c_glib/policy_policy_types.c \
	gen-c_glib/policy_policy_types.h \
	gen-c_glib/policy_policy.c \
	gen-c_glib/policy_policy.h

libtutorialgencglib_la_LIBADD = \
	/usr/local/lib/libthrift_c_glib.la

libtutorialgencglib_la_CFLAGS = \
	$(AM_CFLAGS) -Wno-unused-function

noinst_PROGRAMS = \
	server

server_SOURCES = \
    common.c \
    policy_lang.c \
    policy.c \
    serialize.c \
    vector.c \
    universe/core.c \
    decentral/dcpabe.c \
    revocation/revocation.c \
    drevocation/drevocation.c \
    dacmacs/dacmacs.c \
    drurevo/drurevo.c \
    drurevo/drur_serialize.c \
	user.c \
	center.c \
	authority.c \
	cloud.c \
	policy_service.c \
	main.c

server_LDFLAGS = -lcrypto

server_LDADD = \
	libtutorialgencglib.la \
	/usr/local/lib/libthrift_c_glib.la

gen-c_glib/aa_aa_types.h \
gen-c_glib/aa_aa_types.c \
gen-c_glib/aa_authority.h \
gen-c_glib/aa_authroity.c \
gen-c_glib/ca_ca_types.h \
gen-c_glib/ca_ca_types.c \
gen-c_glib/ca_center.h \
gen-c_glib/ca_center.c \
gen-c_glib/user_user_types.h \
gen-c_glib/user_user_types.c \
gen-c_glib/user_user.h \
gen-c_glib/user_user.c : $(top_srcdir)/thrift/user.thrift
	thrift --gen c_glib -r $<

gen-c_glib/cloud_cloud.h \
gen-c_glib/cloud_cloud.c \
gen-c_glib/cloud_cloud_types.h \
gen-c_glib/cloud_cloud_types.c : $(top_srcdir)/thrift/cloud.thrift
	thrift --gen c_glib -r $<

gen-c_glib/policy_policy_types.c \
gen-c_glib/policy_policy_types.h \
gen-c_glib/policy_policy.c \
gen-c_glib/policy_policy.h : $(top_srcdir)/thrift/policy.thrift
	thrift --gen c_glib -r $<


clean-local:
	$(RM) gen-c_glib/*

servertarget: all
	./server

EXTRA_DIST = \
	common.c \
    policy_lang.c \
    policy.c \
    serialize.c \
    vector.c \
    universe \
    decentral \
    revocation \
    drevocation \
    dacmacs \
    drurevo \
    drurevo \
	user.c \
	center.c \
	authority.c \
	cloud.c \
	policy_service.c \
	main.c