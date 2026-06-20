#pragma once

#include "ArraySequence.hpp"
#include "Sequence.hpp"

template <typename T, typename U>
Sequence<U> *Map(const Sequence<T> &seq, U (*func)(const T &)) {
  MutableArraySequence<U> *result = new MutableArraySequence<U>();
  for (int i = 0; i < seq.GetCount(); ++i) {
    result->append(func(seq.Get(i)));
  }
  return result;
}

template <typename T>
T Reduce(const Sequence<T> &seq, T (*func)(const T &, const T &), T initial) {
  T acc = initial;
  for (int i = 0; i < seq.GetCount(); ++i) {
    acc = func(acc, seq.Get(i));
  }
  return acc;
}

template <typename T>
Sequence<T> *Where(const Sequence<T> &seq, bool (*predicate)(const T &)) {
  MutableArraySequence<T> *result = new MutableArraySequence<T>();
  for (int i = 0; i < seq.GetCount(); ++i) {
    if (predicate(seq.Get(i))) {
      result->append(seq.Get(i));
    }
  }
  return result;
}
