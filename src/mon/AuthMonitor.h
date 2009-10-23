// -*- mode:C++; tab-width:8; c-basic-offset:2; indent-tabs-mode:t -*- 
// vim: ts=8 sw=2 smarttab
/*
 * Ceph - scalable distributed file system
 *
 * Copyright (C) 2004-2006 Sage Weil <sage@newdream.net>
 *
 * This is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License version 2.1, as published by the Free Software 
 * Foundation.  See file COPYING.
 * 
 */

#ifndef __AUTHMONITOR_H
#define __AUTHMONITOR_H

#include <map>
#include <set>
using namespace std;

#include "include/types.h"
#include "msg/Messenger.h"
#include "PaxosService.h"
#include "mon/Monitor.h"

#include "auth/cephx/CephxKeyServer.h"

class MMonCommand;
class MAuth;
class MAuthMon;
class MAuthRotating;

class AuthMonitor : public PaxosService {
  void auth_usage(stringstream& ss);
  vector<KeyServerData::Incremental> pending_auth;
  version_t last_rotating_ver;

  void on_active();

  void create_initial(bufferlist& bl);
  bool update_from_paxos();
  void create_pending();  // prepare a new pending
  void encode_pending(bufferlist &bl);  // propose pending update to peers

  void committed();

  bool preprocess_query(PaxosServiceMessage *m);  // true if processed.
  bool prepare_update(PaxosServiceMessage *m);

  bool preprocess_auth(MAuth *m);

  bool preprocess_auth_rotating(MAuthRotating *m);

  bool preprocess_command(MMonCommand *m);
  bool prepare_command(MMonCommand *m);

  void check_rotate();
 public:
  AuthMonitor(Monitor *mn, Paxos *p) : PaxosService(mn, p), last_rotating_ver(0) {}
  void pre_auth(MAuth *m);
  
  void tick();  // check state, take actions

  void init();
};

#endif
