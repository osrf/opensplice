/*
 *                         OpenSplice DDS
 *
 *   This software and documentation are Copyright 2006 to 2009 PrismTech 
 *   Limited and its licensees. All rights reserved. See file:
 *
 *                     $OSPL_HOME/LICENSE 
 *
 *   for full copyright notice and license terms. 
 *
 */

#ifndef V__ENTRY_H
#define V__ENTRY_H

#include "v_entry.h"

#define v_entryReader(_this) v_reader(v_entry(_this)->reader)

void
v_entryInit (
    v_entry _this,
    v_reader r);

void
v_entryFree (
    v_entry _this);

void
v_entryAddGroup (
    v_entry _this,
    v_group g);

void
v_entryRemoveGroup (
    v_entry _this,
    v_group g);

c_bool
v_entryGroupExists(
    v_entry entry,
    v_group group);

#endif
