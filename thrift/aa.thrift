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

namespace c_glib aa
namespace cpp aa
namespace d aa // "shared" would collide with the eponymous D keyword.
namespace dart aa
namespace java aa
namespace perl aa
namespace php aa
namespace haxe aa
namespace netstd aa

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

struct AttrPair {
    1: binary attr_pk
    2: binary attr_msk
}

service Authority {
    AuthParamter    aasetup     (1: binary pub, 
                                 2: list<string> attributes, 
                                 3: string aid)

    ARevoKey        aakeygen    (1: AuthParamter auth, 
                                 2: binary pub, 
                                 3: binary cert, 
                                 4: list<string> attribtues)

    AttrPair        updatekey   (1: binary auth_msk,
                                 2: binary pub,
                                 3: binary attr_pk,
                                 4: binary attr_msk,
                                 5: string attribute)

}