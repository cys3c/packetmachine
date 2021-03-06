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

#include <unistd.h>

#include "./kernel.hpp"
#include "./packetmachine/property.hpp"
#include "./debug.hpp"

namespace pm {

Kernel::Kernel() : recv_pkt_(0), recv_size_(0) {
  this->callback_.resize(this->dec_.event_size());
}
Kernel::~Kernel() {
}

void* Kernel::thread(void* obj) {
  Kernel* p = static_cast<Kernel*>(obj);
  p->run();
  return nullptr;
}

void Kernel::run() {
  Packet* pkt;
  Payload pd;
  Property prop(&(this->dec_));
  
  while (nullptr != (pkt = this->channel_.pull())) { 
    this->recv_pkt_  += 1;
    this->recv_size_ += pkt->cap_len();

    prop.init(pkt);
    pd.reset(pkt);
    this->dec_.decode(&pd, &prop);

    size_t ev_size = prop.event_idx();
    for (size_t i = 0; i < ev_size; i++) {
      event_id eid = prop.event(i)->id();
      for (auto cb : this->callback_[eid]) {
        cb(prop);
      }
    }
  }
}

void Kernel::proc(Packet* pkt) {
}

bool Kernel::on(const std::string& event_name, Callback& cb) {
  event_id eid = this->dec_.lookup_event_id(event_name);

  if (eid == Event::NONE) {
    return false;
  }

  this->callback_[eid].push_back(cb);
  
  return false;
}


}   // namespace pm
