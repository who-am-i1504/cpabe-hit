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
namespace c_glib cloud
namespace cpp cloud
namespace d cloud // "shared" would collide with the eponymous D keyword.
namespace dart cloud
namespace java cloud
namespace perl cloud
namespace php cloud
namespace haxe cloud
namespace netstd cloud

service Cloud {
    binary updatecph(1: binary cph,
                     2: binary pub,
                     3: binary cuk,
                     4: i32    version,
                     5: string attribute)
}
 