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

#ifndef __PACKETMACHINE_MODULE_HPP__
#define __PACKETMACHINE_MODULE_HPP__

#include <map>
#include <vector>
#include <string>

#include "./packetmachine/object.hpp"
#include "./packetmachine/property.hpp"
#include "./debug.hpp"

namespace pm {

class Decoder;

class ParamDef {
 private:
  mod_id module_id_;
  param_id id_;
  std::string name_;
  std::string local_name_;
  Value*(*constructor_)();

 public:
  ParamDef(const std::string& local_name, Value*(*constructor)());
  ~ParamDef();

  void set_module_id(mod_id id) { this->module_id_ = id; }
  void set_id(param_id id) { this->id_ = id; }
  void set_name(const std::string& name) { this->name_ = name; }

  mod_id module_id() const { return this->module_id_; }
  param_id id() const { return this->id_; }
  const std::string& name() const { return this->name_; }
  Value* new_object() const { return this->constructor_(); }
};

class EventDef {
 private:
  mod_id module_id_;
  event_id id_;
  std::string name_;
  std::string local_name_;

 public:
  EventDef(const std::string& local_name) : local_name_(local_name) {};
  ~EventDef() {};
  void set_module_id(mod_id id) { this->module_id_ = id; }
  void set_id(param_id id) { this->id_ = id; }
  void set_name(const std::string& name) { this->name_ = name; }

  mod_id module_id() const { return this->module_id_; }
  param_id id() const { return this->id_; }
  const std::string& name() const { return this->name_; }
};

typedef std::map<std::string, ParamDef*> ParamMap;
typedef std::map<std::string, EventDef*> EventMap;

class Module {
 private:
  ParamMap param_map_;
  EventMap event_map_;
  Decoder *dec_;
  mod_id id_;
  std::string name_;

 protected:
  static Value* new_value();
  static Value* new_array();
  static Value* new_map();

  const ParamDef* define_param(const std::string& name,
                               Value*(*new_object)() = new_value);
  mod_id lookup_module(const std::string& name);
  param_id lookup_param_id(const std::string& name);

 public:
  static const mod_id NONE = -1;

  Module();
  virtual ~Module();
  virtual void setup() = 0;
  virtual mod_id decode(Payload* pd, Property* prop) = 0;

  mod_id id() const { return this->id_; }
  const std::string& name() const { return this->name_; }

  const EventDef* define_event(const std::string& name);

  ParamMap* param_map() { return &(this->param_map_); }
  EventMap* event_map() { return &(this->event_map_); }
  void set_decoder(Decoder* dec);
  void set_mod_id(mod_id id);
  void set_name(const std::string& name);
};

void build_module_map(std::map<std::string, Module*> *mod_map);

#define INIT_MODULE(CNAME)                                          \
  Module* __new_module_##CNAME##_factory () { return new CNAME(); }

#define NEW_MODULE(CNAME)                       \
  __new_module_##CNAME##_factory ()
  
  
}   // namespace pm

#endif   // __PACKETMACHINE_MODULE_HPP__
