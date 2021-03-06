//--------------------------------------------------------------------------
// Copyright (C) 2014-2016 Cisco and/or its affiliates. All rights reserved.
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU General Public License Version 2 as published
// by the Free Software Foundation.  You may not use, modify or distribute
// this program under any other version of the GNU General Public License.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
//--------------------------------------------------------------------------
// nhttp_module.h author Tom Peters <thopeter@cisco.com>

#ifndef NHTTP_MODULE_H
#define NHTTP_MODULE_H

#include <string>
#include <bitset>

#include "framework/module.h"

#include "nhttp_enum.h"

#define NHTTP_NAME "new_http_inspect"
#define NHTTP_HELP "new HTTP inspector"

struct NHttpParaList
{
public:
    long request_depth;
    long response_depth;
    bool unzip;
    struct UriParam
    {
    public:
        UriParam();
        ~UriParam() { delete[] unicode_map; }

        bool percent_u;
        bool utf8;
        bool utf8_bare_byte;
        bool iis_unicode;
        uint8_t* unicode_map = nullptr;
        bool backslash_to_slash;
        bool plus_to_space;
        bool simplify_path;
        std::bitset<256> bad_characters;
        std::bitset<256> unreserved_char;
        NHttpEnums::CharAction uri_char[256];
    };
    UriParam uri_param;
#ifdef REG_TEST
    bool test_input;
    bool test_output;
    long print_amount;
    bool print_hex;
#endif
};

class NHttpModule : public Module
{
public:
    NHttpModule() : Module(NHTTP_NAME, NHTTP_HELP, nhttp_params) { }
    ~NHttpModule() { delete params; }
    bool begin(const char*, int, SnortConfig*) override;
    bool end(const char*, int, SnortConfig*) override;
    bool set(const char*, Value&, SnortConfig*) override;
    unsigned get_gid() const override { return NHttpEnums::NHTTP_GID; }
    const RuleMap* get_rules() const override { return nhttp_events; }
    const NHttpParaList* get_once_params()
    {
        NHttpParaList* ret_val = params;
        params = nullptr;
        return ret_val;
    }

private:
    static const Parameter nhttp_params[];
    static const RuleMap nhttp_events[];
    NHttpParaList* params = nullptr;
};

#endif

