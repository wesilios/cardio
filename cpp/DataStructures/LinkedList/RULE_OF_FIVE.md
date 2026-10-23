# Rule of five for the linked lists

Notes for later. The rule of three is done for all four lists. The rule of five (move operations) is done for `SinglyLinkedList` and `DoublyLinkedList` only.

## Why the lists need it

Each list owns heap nodes through raw pointers (`head_`, `tail_`) and frees them in its destructor. The copy operations the compiler generates copy those pointers, not the nodes, so two lists end up owning the same nodes. The second destructor then reads and frees memory that is already gone (heap-use-after-free or a double free under ASan).

| Rule | Special members | Purpose |
| --- | --- | --- |
| Three | destructor, copy constructor, copy assignment | a copy owns its own nodes (deep copy) |
| Five | the three above, plus move constructor and move assignment | hand the nodes over in O(1) without allocating |

If a class declares a copy constructor or a destructor, the compiler does **not** generate move operations. `std::move(list)` then compiles but quietly copies. That's why the move tests check node addresses, not just values.

## Agreed behaviour (pinned by the tests)

- **Copy:** a deep copy. Changing either list never changes the other, and none of the nodes are shared.
- **Copy assignment:** replaces whatever the target held (longer, shorter or empty), survives `a = a`, returns `*this`, and chains (`c = b = a`).
- **Move:** the target takes the source's nodes (same addresses). The source is left **empty and reusable**.
- **Noexcept:** both move operations are `noexcept`, so `std::vector<List>` moves elements when it grows, instead of copying them.

## Status

| List | Rule of three | Rule of five | Move tests |
| --- | --- | --- | --- |
| `SinglyLinkedList` | ✅ | ✅ | enabled |
| `DoublyLinkedList` | ✅ | ✅ | enabled |
| `CircularSinglyLinkedList` | ✅ | ❌ | `DISABLED_` |
| `CircularDoublyLinkedList` | ✅ | ❌ | `DISABLED_` |

To finish a circular list:

1. Add a move constructor and a move assignment.
2. In `tests/<List>Tests.cpp`, remove the `DISABLED_` prefixes in the "Move (rule of five)" section.
3. Run the sanitizer build.

To try the move tests before enabling them:

```bash
./build-asan/runTests --gtest_also_run_disabled_tests --gtest_filter='CircularSinglyLinkedList.*Move*'
```

## Open questions to consider later

1. **Self-move (`a = std::move(a)`).** With move-and-swap, the list looks like it ends up unchanged. Should a test lock that in? The standard library only promises "valid but unspecified" here.
2. **Reading members or calling getters in the move constructor.** Singly uses `other.size()` and `other.getHead()`, and Doubly reads `other.size_` and `other.head_`. Both work; pick one style.
3. **`Iterator::operator*` is not `const`.** So `*it` doesn't compile on a `const Iterator`. Should it?
4. **Copying a circular list with a counting iterator.** Make sure the copy constructor stops after `size()` nodes and that the copy's `tail->next` points to its own `head`, not the source's.
5. **The same bug lands in the next containers.** The dynamic array planned for the stack, queue and `Dictionary` will own a raw `T *` and needs the same five members.
