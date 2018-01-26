#ifndef HASHTABLE_H_
#define HASHTABLE_H_

/****** Class Definitions ******/

constexpr extern size_t HASH_SPACE;

template <class DataT>
class HashCell {
public:
  DataT     data;
  HashCell *next = nullptr;

  class iterator {
  public:
    HashCell   *objcur;

  public:
    iterator();
    ~iterator();
    iterator &operator=(const iterator &);
    bool operator==(const iterator &);
    iterator &operator++();
    HashCell &operator*();
  };

public:
  inline bool operator<=(const LogDate &);
  inline bool operator<(const LogDate &);
  inline bool operator>(const LogDate &);
  inline bool operator>=(const LogDate &);
};

#endif
