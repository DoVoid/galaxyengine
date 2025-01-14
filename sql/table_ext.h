/* Copyright (c) 2018, 2021, Alibaba and/or its affiliates. All rights reserved.
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License, version 2.0,
   as published by the Free Software Foundation.
   This program is also distributed with certain software (including
   but not limited to OpenSSL) that is licensed under separate terms,
   as designated in a particular file or component or in included license
   documentation.  The authors of MySQL/Apsara GalaxyEngine hereby grant you an
   additional permission to link the program and your derivative works with the
   separately licensed software that they have included with
   MySQL/Apsara GalaxyEngine.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License, version 2.0, for more details.
   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301  USA */


#ifndef TABLE_EXT_INCLUDED
#define TABLE_EXT_INCLUDED

#include "sql/sql_lex_ext.h" // Table_snapshot
#include "my_dbug.h"

struct TABLE;
struct LEX;

namespace im {

typedef enum {
  SNAPSHOT_NONE,
  AS_OF_TIMESTAMP,
  AS_OF_SCN,
  AS_OF_GCN
} Snapshot_type;

/*
  Snapshot clause info.
*/
class Snapshot_info_t {
  Snapshot_type type;

  union {
    uint64_t ts;
    uint64_t scn;
    uint64_t gcn;
  } value;

 public:
  Snapshot_info_t() : type(SNAPSHOT_NONE) {}

  bool valid() const { return (type != SNAPSHOT_NONE); }

  Snapshot_type get_type() const { return type; }

  uint64_t get_asof_timestamp() const {
    DBUG_ASSERT(type == AS_OF_TIMESTAMP);
    return value.ts;
  }

  uint64_t get_asof_scn() const {
    DBUG_ASSERT(type == AS_OF_SCN);
    return value.scn;
  }

  uint64_t get_asof_gcn() const {
    DBUG_ASSERT(type == AS_OF_GCN);
    return value.gcn;
  }

  void reset() { type = SNAPSHOT_NONE; }

  void set_timestamp(uint64_t ts) {
    DBUG_ASSERT(type == SNAPSHOT_NONE);
    type = AS_OF_TIMESTAMP;
    value.ts = ts;
  }

  void set_scn(uint64_t scn) {
    DBUG_ASSERT(type == SNAPSHOT_NONE);
    type = AS_OF_SCN;
    value.scn = scn;
  }

  void set_gcn(uint64_t gcn) {
    DBUG_ASSERT(type == SNAPSHOT_NONE);
    type = AS_OF_GCN;
    value.gcn = gcn;
  }
};

/*
  Reset snapshot, increase the snapshot table count.
*/
extern void init_table_snapshot(TABLE* table, THD *thd);

/*
  Evaluate table snapshot expressions.
*/
extern bool evaluate_snapshot(THD *thd, const LEX *lex);

} // namespace im

#endif // table_ext.h

