#pragma once

template <typename T> class ICollection {
public:
  virtual ~ICollection() {}
  virtual int GetCount() const = 0;
  virtual const T &Get(int index) const = 0;
};