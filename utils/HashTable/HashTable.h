#ifndef HASHTABLE_H_
#define HASHTABLE_H_

/****** Class Definitions ******/

constexpr extern size_t HASH_SPACE;

template <class DataT>
class HashCell {
public:
  DataT     data;
  HashCell *next    = nullptr;

  class iterator {
  public:
    HashCell   *objcur  = nullptr;
  public:
    inline iterator &operator= (const iterator &other) {
      this->objcur = other.objcur; return *this;
    }
    inline bool operator==(const iterator &other) {
      return this->objcur == other.objcur;
    }
    inline HashCell &operator*() { return *objcur; }
    inline ???? operator->() // TODO
  };

public:
  HashCell();
  HashCell(const DataT &);
  inline bool operator<=(const DataT &other) {
    return this->data <= other.data;
  }
  inline bool operator< (const DataT &other) {
    return this->data <  other.data;
  }
  inline bool operator> (const DataT &other) {
    return this->data >  other.data;
  }
  inline bool operator>=(const DataT &other) {
    return this->data >= other.data;
  }

  inline iterator begin() { // time axis
    iterator ret; ret.objcur = this; return ret;
  }
  inline iterator end() {
    iterator ret; ret.objcur = nullptr; return ret;
  }
  inline iterator &operator++() {
    if (objcur != nullptr) { objcur = objcur->next; }
    return *this;
  }
};

#endif
