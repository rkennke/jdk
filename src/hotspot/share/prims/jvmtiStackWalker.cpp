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

#include "prims/jvmtiStackWalker.hpp"
#include "prims/jvmtiThreadLocal.hpp"
#include "runtime/javaThread.hpp"
#include "runtime/thread.hpp"

static bool is_excluded(JavaThread* jt) {
  return jt->is_hidden_from_external_view() ||
         jt->jfr_thread_local()->is_excluded() ||
         jt->is_JfrRecorder_thread();
}

static JavaThread* get_java_thread_if_valid() {
  Thread* raw_thread = Thread::current_or_null_safe();
  if (raw_thread == nullptr) {
    // probably while shutting down
    return nullptr;
  }
  assert(raw_thread->is_Java_thread(), "invariant");
  JavaThread* jt = JavaThread::cast(raw_thread);
  if (is_excluded(jt) || jt->is_exiting()) {
    return nullptr;
  }
  return jt;
}

jvmtiError JVMTIStackWalker::request_stack_trace(jthread thread, jvmtiStackFrameCallback stack_frame_callback, jint max_depth, void* ucontext, const void* user_data) {
  JavaThread* jt = get_java_thread_if_valid();
  if (jt == nullptr) {
    return JVMTI_ERROR_INVALID_THREAD;
  }

  JVMTIThreadLocal& tl = jt->jvmti_thread_local();
  JVMTIStackWalkerQueue queue = tl.jvmti_stack_walker_queue();
  return JVMTI_ERROR_NONE;
}
