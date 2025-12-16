/*
 * Copyright (c) 2025, Datadog, Inc. All rights reserved.
 * Copyright (c) 2025 SAP SE. All rights reserved.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * This code is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 only, as
 * published by the Free Software Foundation.
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * version 2 for more details (a copy is included in the LICENSE file that
 * accompanied this code).
 *
 * You should have received a copy of the GNU General Public License version
 * 2 along with this work; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Please contact Oracle, 500 Oracle Parkway, Redwood Shores, CA 94065 USA
 * or visit www.oracle.com if you need additional information or have any
 * questions.
 *
 */

#ifndef SHARE_PRIMS_JVMTISTACKWALKERQUEUE_HPP
#define SHARE_PRIMS_JVMTISTACKWALKERQUEUE_HPP

class JVMTIStackWalkerQueue {
  enum StackWalkerQueueLockState {
    UNLOCKED,
    LOCKED
  };
  volatile StackWalkerQueueLockState _queue_lock;

public:
  JVMTIStackWalkerQueue() : _queue_lock(UNLOCKED) {}

  // Non-blocking for signal-safety. If the lock can't be acquired,
  // we return immediately with a failure.
  bool try_acquire_lock();
  // Blocking variant, it's only acquired when the thread is at a safepoint.
  void acquire_lock();
  // Releases the lock.
  void release_lock();

};

#endif // SHARE_PRIMS_JVMTISTACKWALKERQUEUE_HPP