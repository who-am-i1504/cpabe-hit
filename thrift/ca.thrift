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

namespace c_glib ca
namespace cpp ca
namespace d ca // "shared" would collide with the eponymous D keyword.
namespace dart ca
namespace java ca
namespace perl ca
namespace php ca
namespace haxe ca
namespace netstd ca

struct CAParamter {
    1: binary pub
    2: binary msk
}

struct UserRegister {
    1: binary gpk
    2: binary gsk
    3: binary UserCert
    4: list<string> attributes
    5: i32 uid
}

struct URevoKey {
    1: i32 m
    2: binary K0
    3: binary K1
    4: binary K2
    5: binary KJ
    6: map<string, binary> Kijx
    7: list<string> attributes
}

service Center {
    CAParamter casetup(1: i32 N, 2: i32 bytesize)
    UserRegister user_register( 1: binary pub,
                                2: string uid, 
                                3: list<string> attributes)
    URevoKey cakeygen(1: CAParamter param, 
                      2: list<string> attributes, 
                      3: i32 uid)
}