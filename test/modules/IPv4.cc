/*
 * Copyright (c) 2016 Masayoshi Mizutani <mizutani@sfc.wide.ad.jp> All
 * rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "./fixtures.hpp"

TEST_F(ModuleTesterData1, IPv4_count) {
  int count_ipv4_saddr = 0;
  const pm::Property* p;
  while (nullptr != (p = get_property())) {
    if ("104.80.179.146" == p->value("IPv4.src").ip4()) {
      count_ipv4_saddr++;
    }
  }
  EXPECT_EQ(270, count_ipv4_saddr);
}

TEST_F(ModuleTesterData1, IPv4_packet) {
  const pm::Property* p;
  p = get_property();   // # packet #1

  EXPECT_EQ(4,      p->value("IPv4.ver").uint());
  EXPECT_EQ(20,     p->value("IPv4.hdr_len").uint());
  EXPECT_EQ(0,      p->value("IPv4.tos").uint());
  EXPECT_EQ(40,     p->value("IPv4.total_len").uint());
  EXPECT_EQ(0x4d45, p->value("IPv4.id").uint());
  EXPECT_EQ(0,      p->value("IPv4.offset").uint());
  EXPECT_EQ(64,     p->value("IPv4.ttl").uint());
  EXPECT_EQ(6,      p->value("IPv4.proto").uint());
  EXPECT_EQ(0x05de, p->value("IPv4.chksum").uint());
  EXPECT_EQ("10.139.96.169", p->value("IPv4.src").ip4());
  EXPECT_EQ("192.30.252.90", p->value("IPv4.dst").ip4());
}

