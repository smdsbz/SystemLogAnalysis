#ifndef FPTREE_H_
#define FPTREE_H_

#include <iostream>
using std::cout; using std::cin; using std::endl;

#include <string>
using std::string;

#include <vector>
using std::vector;

#include <stdexcept>

#include "../LogClass/LogClass.h"
#include "../HashTable/HashTable.h"
#include "../StorageGraph/StorageGraph.h"


class FPTree {
public:

  class _Node {
  public:
    LogMessage *entity = nullptr;
    size_t  freq   = 0;
    _Node  *child  = nullptr;
    _Node  *brother= nullptr;
    _Node  *next_occur = nullptr;

  public:

    explicit _Node(LogMessage *p) {
      if (p == nullptr) {
        throw std::runtime_error("FPTree::_Node() Empty node!");
      }
      this->entity = p;
      return;
    }

    inline _Node &operator++() { this->freq += 1; return *this; }

  };    // _Node

  class _Header {
  public:
    LogMessage *entity = nullptr;
    size_t  freq   = 0;
    _Node  *occur  = nullptr;

  public:

    _Header(LogMessage *p, size_t f) {
      if (p == nullptr) {
        throw std::runtime_error("FPTree::_Header() Empty header!");
      }
      this->entity = p;
      this->freq   = f;
      return;
    }

    _Header &operator= (const _Header &other) {
      this->entity = other.entity;
      this->freq   = other.freq;
      this->occur  = other.occur;
      return *this;
    }

    inline bool operator==(const _Header &other) {
      return this->entity == other.entity;
    }

    inline LogMessage *operator->() {
      if (this->entity == nullptr) {
        throw std::runtime_error("FPTree::_Header::operator-> `entity` field is empty!");
      }
      return this->entity;
    }

  };    // _Header


  vector<_Header>  headers;
  _Node           *nodes = nullptr;

public:

  FPTree() { /* pass */ ; }

  _Header &operator[](LogMessage *p) {
    // check if `p` is in `headers`
    for (auto &each : this->headers) {
      if (*each.entity == *p) { return each; }
    }   // else - `p` not in `headers` yet
    throw std::overflow_error("FPTree::operator[LogMessage] Not found!");
  }

  _Header &_add_header(LogMessage *p, size_t f) {
    // first header
    if (this->headers.empty()) {
      this->headers.push_back(_Header(p, f));
      return this->headers[0];
    }
    // not first - consider insertion place
    // insert to tail
    if (f < this->headers.back().freq) {
      this->headers.push_back(_Header(p, f));
      return this->headers.back();
    }
    // insert at first / middle
    for (size_t idx = 0, range = this->headers.size();
         idx != range; ++idx) {
      if (f < this->headers[idx].freq) { continue; }
      this->headers.insert(this->headers.cbegin() + idx, _Header(p, f));
      return this->headers[idx];
    }
    throw std::runtime_error("FPTree::_add_header() Cannot find insert position");
  }

  FPTree &_first_run(Storage &s) {
    for (size_t idx = 0, range = s.messages->space;
         idx != range; ++idx) {
      if (s.messages->table[idx].occupied()) {
        for (auto pcell = s.messages->table + idx;
             pcell; pcell = pcell->next) {
          this->_add_header(&pcell->data, pcell->count);
        }   // for all isotopes
      } // if has content
    }   // for all cell in table
    return *this;
  } // _first_run

};

#endif

