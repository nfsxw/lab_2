#pragma once

#include "ArraySequence.hpp"
#include "Bit.hpp"
#include "Sequence.hpp"
#include <stdexcept>
#include <string>

class BitSequence : public Sequence<Bit> {
private:
  MutableArraySequence<Bit> data;

public:
  BitSequence() : data() {}
  BitSequence(const std::string &bits) : data() {
    for (char c : bits) {
      if (c == '0')
        data.append(Bit(false));
      else if (c == '1')
        data.append(Bit(true));
      else
        throw std::invalid_argument("BitSequence: invalid char in string");
    }
  }
  BitSequence(const Bit *source, int amount) : data(source, amount) {}
  BitSequence(const BitSequence &other) : data(other.data) {}

  const Bit &getFirst() const override { return data.getFirst(); }
  const Bit &getLast() const override { return data.getLast(); }
  const Bit &get(int index) const override { return data.get(index); }
  int getLength() const override { return data.getLength(); }

  Sequence<Bit> *append(const Bit &value) override {
    BitSequence *result = new BitSequence(*this);
    result->data.append(value);
    return result;
  }

  Sequence<Bit> *prepend(const Bit &value) override {
    BitSequence *result = new BitSequence(*this);
    result->data.prepend(value);
    return result;
  }

  Sequence<Bit> *insertAt(const Bit &value, int index) override {
    BitSequence *result = new BitSequence(*this);
    result->data.insertAt(value, index);
    return result;
  }

  Sequence<Bit> *remove(int index) override {
    BitSequence *result = new BitSequence(*this);
    result->data.remove(index);
    return result;
  }

  Sequence<Bit> *getSubsequence(int start, int end) const override {
    BitSequence *result = new BitSequence();
    for (int i = start; i <= end; ++i) {
      result->data.append(data.get(i));
    }
    return result;
  }

  Sequence<Bit> *concat(const Sequence<Bit> &other) const override {
    BitSequence *result = new BitSequence(*this);
    for (int i = 0; i < other.getLength(); ++i) {
      result->data.append(other.get(i));
    }
    return result;
  }

  BitSequence *And(const BitSequence &other) const {
    if (data.getLength() != other.getLength())
      throw std::invalid_argument("BitSequence->And: lengths must be equal");
    BitSequence *result = new BitSequence();
    for (int i = 0; i < data.getLength(); ++i) {
      result->data.append(data.get(i) & other.get(i));
    }
    return result;
  }

  BitSequence *Or(const BitSequence &other) const {
    if (data.getLength() != other.getLength())
      throw std::invalid_argument("BitSequence->Or: lengths must be equal");
    BitSequence *result = new BitSequence();
    for (int i = 0; i < data.getLength(); ++i) {
      result->data.append(data.get(i) | other.get(i));
    }
    return result;
  }

  BitSequence *Xor(const BitSequence &other) const {
    if (data.getLength() != other.getLength())
      throw std::invalid_argument("BitSequence->Xor: lengths must be equal");
    BitSequence *result = new BitSequence();
    for (int i = 0; i < data.getLength(); ++i) {
      result->data.append(data.get(i) ^ other.get(i));
    }
    return result;
  }

  BitSequence *Not() const {
    BitSequence *result = new BitSequence();
    for (int i = 0; i < data.getLength(); ++i) {
      result->data.append(~data.get(i));
    }
    return result;
  }

  virtual ~BitSequence() override {}
};