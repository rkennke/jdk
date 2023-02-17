/*
 * Copyright (c) 2022, Oracle and/or its affiliates. All rights reserved.
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

#ifndef SHARE_RUNTIME_LOCKSTACK_HPP
#define SHARE_RUNTIME_LOCKSTACK_HPP

#include "oops/oopsHierarchy.hpp"
#include "utilities/globalDefinitions.hpp"
#include "utilities/sizes.hpp"

class Thread;
class OopClosure;

class LockStack {
  friend class VMStructs;
private:
  static const size_t INITIAL_CAPACITY = 1;

  static inline oop decode_oop(intptr_t entry) {
    return cast_to_oop(entry & ~OOP_MASK);
  }

  static inline intptr_t encode_oop(oop o) {
    intptr_t entry = cast_from_oop<intptr_t>(o);
    assert(decode_recursion(entry) == 0, "sanity");
    return entry;
  }

  static inline int decode_recursion(intptr_t entry) {
    return static_cast<int>(entry & OOP_MASK);
  }

  intptr_t* _base;
  intptr_t* _limit;
  intptr_t* _current;

  void grow(size_t min_capacity);

  void validate(const char* msg) const PRODUCT_RETURN;
public:
#ifdef _LP64
  static const intptr_t OOP_MASK = 7;
#else
  static const intptr_t OOP_MASK = 3;
#endif

  static ByteSize current_offset()    { return byte_offset_of(LockStack, _current); }
  static ByteSize base_offset()       { return byte_offset_of(LockStack, _base); }
  static ByteSize limit_offset()      { return byte_offset_of(LockStack, _limit); }

  static void ensure_lock_stack_size(intptr_t* _required_limit);

  LockStack();
  ~LockStack();

  inline void push(oop o);
  inline oop pop();
  inline void remove(oop o);

  inline bool contains(oop o) const;

  inline bool try_enter_recursive(oop obj);
  inline bool try_exit_recursive(oop obj);
  inline intx get_recursions(oop obj);

  // GC support
  inline void oops_do(OopClosure* cl);

};

#endif // SHARE_RUNTIME_LOCKSTACK_HPP
