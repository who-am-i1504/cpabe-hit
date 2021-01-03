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

namespace c_glib user
namespace cpp user
namespace d user // "shared" would collide with the eponymous D keyword.
namespace dart user
namespace java user
namespace perl user
namespace php user
namespace haxe user
namespace netstd user

struct UserPrvKey {
    1: binary gpk
    2: binary gsk
    3: binary cert
    4: map<string, binary> sks
    5: binary cask
    6: list<string> attributes
}

service UserService {
    binary encrypt(1: binary pub, 
                   2: list<binary> attr_pks,
                   3: string policy,
                   4: list<i32> revo_list,
                   5: i32 uindex,
                   6: binary mes)
    
    binary decrypt(1: binary cph, 
                   2: binary pub, 
                   3: UserPrvKey prv)
}
