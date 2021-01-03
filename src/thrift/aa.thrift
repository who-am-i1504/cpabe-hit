/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements. See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership. The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License. You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied. See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

/**
 * This Thrift file can be included by other Thrift files that want to share
 * these definitions.
 */

namespace c_glib attr_auth
namespace cpp attr_auth
namespace d attr_auth // "shared" would collide with the eponymous D keyword.
namespace dart attr_auth
namespace java attr_auth
namespace perl attr_auth
namespace php attr_auth
namespace haxe attr_auth
namespace netstd attr_auth

struct AuthParamter {
    1: string aid
    2: binary auth_pk
    3: binary auth_msk
    4: map<string, binary> attr_pks
    5: map<string, binary> attr_msks
    6: list<string> attributes
}

struct ARevoKey {
    1: string aid
    2: binary K
    3: binary L
    4: binary R
    5: map<string, binary> kx
    6: list<string> attributes
}

service AAService {
    AuthParamter aasetup(1: binary pub, 
                         2: list<string> attributes, 
                         3: string aid)

    ARevoKey aakeygen(1: AuthParamter auth, 
                      2: binary pub, 
                      3: binary cert, 
                      4: list<string> attribtues)
}