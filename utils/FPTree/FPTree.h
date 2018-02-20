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
    _HashCell_LogMessage *entity = nullptr;
    size_t  occur  = 0;
    _Node  *child  = nullptr;
    _Node  *brother= nullptr;
    /* _Node  *next_occur = nullptr; */

  public:

    explicit _Node(_HashCell_LogMessage *p) {
      if (p == nullptr) {
        throw std::runtime_error("FPTree::_Node() Empty node!");
      }
      this->entity = p;
      return;
    }

    inline _Node &operator++() { this->occur += 1; return *this; }

    _Node *in_brothers(_HashCell_LogMessage *p) {
      _Node *pnode = this;
      for (; pnode != nullptr; pnode = pnode->brother) {
        if (pnode->entity == p) { break; }
      }
      return pnode; // NOTE: Including returning `nullptr`
    }

    _Node *in_children(_HashCell_LogMessage *p) {
      return this->child->in_brothers(p);
    }

    _Node *last_brother() {
      _Node *pnode = this;
      for (; pnode->brother != nullptr; pnode = pnode->brother) { ; }
      return pnode;
    }

  };    // _Node

  class _Header {
  public:
    _HashCell_LogMessage *entity = nullptr;
    size_t  freq   = 0;
    _Node  *occur  = nullptr;

  public:

    _Header(_HashCell_LogMessage *p, size_t f) {
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

    inline _HashCell_LogMessage *operator->() {
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

  _Header &operator[](_HashCell_LogMessage *p) {
    // check if `p` is in `headers`
    for (auto &each : this->headers) {
      if (each.entity->data == p->data) { return each; }
    }   // else - `p` not in `headers` yet
    throw std::overflow_error("FPTree::operator[LogMessage] Not found!");
  }

  _Header &_add_header(_HashCell_LogMessage *p, size_t f) {
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

  FPTree &_first_run(Storage &s, size_t valve_freq=3) {
    for (size_t idx = 0, range = s.messages->space;
         idx != range; ++idx) {
      if (s.messages->table[idx].occupied()) {
        for (auto pcell = s.messages->table + idx;
             pcell; pcell = pcell->next) {
          if (pcell->count < valve_freq) { continue; }
          /* this->_add_header(&pcell->data, pcell->count); */
          this->_add_header(pcell, pcell->count);
        }   // for all isotopes
      } // if has content
    }   // for all cell in table
    return *this;
  } // _first_run

  FPTree &_add_pattern(vector<_Header> pat) {
    // first pattern
    if (this->nodes == nullptr) {
      // simply convert vector to link-list
      try {
        this->nodes = new _Node(pat[0].entity);   // add first
      } catch (const std::bad_alloc &e) {
        cout << "Space allocation failed!" << endl;
        throw e;
      }
      auto prev_node = *this->nodes;
      ++prev_node;  // inc `occur`
      for (size_t idx = 1, range = pat.size();
           idx != range;
           ++idx, prev_node = *prev_node.child) {
        try {
          prev_node.child = new _Node(pat[idx].entity);
        } catch (const std::bad_alloc &e) {
          cout << "Space allocation failed!" << endl;
          throw e;
        }
        ++prev_node;    // inc `occur`
      }
    } else {    // not first pattern
      _Node *current_layer = this->nodes;
      _Node *cursor_node = nullptr;
      for (auto &each : pat) {
        cursor_node = current_layer->in_brothers(each.entity);
        if (cursor_node == nullptr) {   // node not in fp-tree yet
          // TODO
        }
      }
    }
  } // _add_pattern

  /* FPTree &_second_run() { */
  /*   for (auto &header : this->headers) { */
  /*     // TODO */
  /*   } */
  /* } */

};

#endif

