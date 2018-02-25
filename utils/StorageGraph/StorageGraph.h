#ifndef STORAGEGRAPH_H_
#define STORAGEGRAPH_H_

#include <iostream>
using std::cout; using std::cin; using std::endl;

#include <string>
using std::string; using std::stoul;

#include <vector>
using std::vector;

#include <fstream>
using std::fstream; using std::getline;

#include <regex>
using std::regex; using std::regex_match;
using std::smatch; using std::ssub_match;

#include <stdexcept>

#include "../LogClass/LogClass.h"
#include "../HashTable/HashTable.h"
#include "../UIGadgets/UIGadgets.h"
using UIGadgets::ProgressBar; using UIGadgets::show_pause;
using UIGadgets::get_decision;

/****** StorageGraph : Class Declaration and Definitioin ******/

class Storage {
public:
  MessageTable     *messages = nullptr;
  SenderTable      *senders  = nullptr;

public:

  inline Storage(size_t msgtable_size = 5000,
                 size_t sdrtable_size = 200) {
    messages = new MessageTable(msgtable_size);
    senders = new SenderTable(sdrtable_size);
    return;
  }

  ~Storage() {
    // NOTE: Only have to clean `LogRecord`s, `Cell`s will be handled by
    //       `Table`s
    LogRecord *beg = this->messages->global_begin;
    LogRecord *to_free = nullptr;
    while (beg != nullptr) {
      to_free = beg;
      beg = beg->time_suc;
      delete to_free;
    }
    return;
  }

  LogRecord &_time_sequence_promised_add(const string &log) {
    // prepare `LogMessage` content
    auto msg = LogMessage();
    try {
      msg = LogMessage(log, false);
    } catch (const std::runtime_error &e) {
      // log passed in can't match regex
      throw e;
    }
    // prepare `LogRecord` content
    LogRecord *prec = nullptr;
    try {
      prec = new LogRecord(log, true); // NOTE: `LogDate` included!
    } catch (const std::bad_alloc &e) {
      std::cerr << "Log Memory! Space allocation failed!" << endl;
      throw e;
    }
    try {
      // NOTE: The following two lines may fail due to cell existing,
      //       but it actually won't make any difference! 'Cause it ensures you
      //       that the cells exist anyway, and it even finds the cell for
      //       you (returned as reference to cell)!
      //       Just pretend that every `insert()` or `link()` is the first
      //       call to that operand.
      auto &msg_cell = this->messages->insert(msg);
      // associate `LogRecord` and `LogMessage`
      prec->set_message(msg_cell.data);
      auto &sndr_cell = this->senders->link(*prec);
      // add `LogRecord` to `Storage`
      // NOTE: The following tow lines requires that `log` passed hasn't
      //       appeared before.
      this->messages->join_rec_to_end(prec);    // set `time_suc`
      msg_cell.join_rec_to_end(prec);           // set `msg_suc`, and inc `count`
      sndr_cell.join_rec_to_end(prec);          // set `sender_suc`
    } catch (const std::bad_alloc &e) { throw e; }
    return *prec;
  }

  size_t is_repeat(const string &line) {
    try {
      auto re = regex("^--- last message repeated ([0-9]+) time[s]? ---$");
      smatch matches;
      regex_match(line, matches, re);
      if (matches.size() != 2) {
        return 0;
      } else { return stoul(matches[1].str()); }
    } catch (const std::exception &e) { return 0; }
  }

  LogRecord &_repeat_last_rec() {
    LogRecord *prec = nullptr;
    try {
      prec = new LogRecord(*this->messages->global_end);
    } catch (const std::bad_alloc &e) { throw e; }
    auto &msg = *prec->message;
    auto &msg_cell = this->messages->insert(msg);   // won't insert, only find
    auto &sndr_cell = this->senders->link(*prec);
    this->messages->join_rec_to_end(prec);
    msg_cell.join_rec_to_end(prec);
    sndr_cell.join_rec_to_end(prec);
    return *prec;
  }

  Storage &read_from_file(const string &filename) {
    fstream file;
    // get filesize - for progress bar
    size_t char_cnt = 0;
    file.open(filename, std::ios::binary | std::ios::in);
    if (file.is_open()) {
      auto fbeg_pos = file.tellg();
      file.seekg(0, std::ios::end);
      auto fend_pos = file.tellg();
      char_cnt = (fend_pos - fbeg_pos) / sizeof(char);
      file.close();
    } else {    // failed to open file
      throw std::runtime_error(string("Storage::read_from_file() Failed to ")
          + "open file \"" + filename + "\"!");
    }
    // read content
    file.open(filename, std::ios::in);
    if (file.is_open()) {
      // NOTE: Some log may span across multiple lines. So you have to store
      //       the previous line, in case you have to append to it later.
      string line;
      LogRecord *prec = nullptr;    // HACK: But instead, I'll store the
                                    //       `LogRecord` for convenience.
      LogMessage msgbuf;
      size_t cur_char_cnt = 0;
      size_t log_cnt = 0;
      auto pbar = ProgressBar(80);
      pbar.draw_on_current_line(0);
      while (getline(file, line)) {
        cur_char_cnt += line.length();
        if (log_cnt % 500 == 0) {   // NOTE: Display too often may cause
                                    //       performance issue!
          pbar.draw_on_current_line(cur_char_cnt * 100 / char_cnt);
        }
        try {
          msgbuf = LogMessage(line);
        } catch (const std::runtime_error &e) {
          // cannot match regex for LogMessage:
          //   1. false file
          //   2. last log spanned
          //   3. --- last message repeated ... ----
          if (prec == nullptr) {    // case 1: false file
            throw std::runtime_error("Storage::read_from_file() Input file is "
                "not standard log text file!");
          } else {
            // case 3: --- last message repeated ... ---
            size_t repeat_times = is_repeat(line);
            if (repeat_times != 0) {
              for (size_t t = 0; t != repeat_times; ++t) {
                prec = &(this->_repeat_last_rec());
              }
              continue;
            }
            // case 2: last log spanned
            prec->message->append_msg(line);
            continue;
          }
        }
        // legit log! added to msgbuf
        prec = &(this->_time_sequence_promised_add(line));
        log_cnt += 1;
      } // end of input while-loop
      file.close();
      pbar.draw_done();
      /* cout << log_cnt << " logs read!" << endl; */
    } else {    // failed to open
      throw std::runtime_error(string("Storage::read_from_file() Failed to ")
          + "open file \"" + filename + "\"!");
    }
    return *this;
  }

  vector<_HashCell_LogMessage *> query_on_message(const string &in,
        const bool fuzzy=false) {
    auto retvec = vector<_HashCell_LogMessage *>();
    if (fuzzy == true) {    // fuzzy search: only the sub-string is needed
                            // have to search the entire table!
      for (size_t spi = 0, space = this->messages->space;
           spi != space; ++spi) {
        // do job if not empty slot
        if (this->messages->table[spi].occupied()) {
          // iter through cell chain
          for (auto pcell = this->messages->table + spi;
               pcell != nullptr;
               pcell = pcell->next) {
            // append to retvec if find matched patter
            if (pcell->_repr().find(in) != string::npos) {
              retvec.push_back(pcell);
            }
          } // iter through cell chain
        }   // else - empty `HashCell` slot, do *NOTHING*
      } // for-loop over `MessageTable` headers
    } else {    // no fuzzy
      try {
        auto pcell = &((*(this->messages))[in]);
        for (; pcell != nullptr; pcell = pcell->next) {
          if (pcell->_repr() == in) {
            retvec.push_back(pcell);
          }
        }   // for on cell chain
      } catch (const std::overflow_error &e) {
        return retvec;
      }
    }
    return retvec;
  }

  vector<_HashCell_string *> query_on_sender(const string &in,
        const bool fuzzy=false) {
    auto retvec = vector<_HashCell_string *>();
    if (fuzzy == true) {    // fuzzy search: only the sub-string is needed
                            // have to search the entire table!
      for (size_t spi = 0, space = this->senders->space;
           spi != space; ++spi) {
        // do job if not empty slot
        if (this->senders->table[spi].occupied()) {
          // iter through cell chain
          for (auto pcell = this->senders->table + spi;
               pcell != nullptr;
               pcell = pcell->next) {
            // append to retvec if find matched patter
            if (pcell->_repr().find(in) != string::npos) {
              retvec.push_back(pcell);
            }
          } // iter through cell chain
        }   // else - empty `HashCell` slot, do *NOTHING*
      } // for-loop over `MessageTable` headers
    } else {    // no fuzzy
      try {
        auto pcell = &((*(this->senders))[in]);
        for (; pcell != nullptr; pcell = pcell->next) {
          if (pcell->_repr() == in) {
            retvec.push_back(pcell);
          }
        }   // for on cell chain
      } catch (const std::overflow_error &e) {
        return retvec;
      }
    }
    return retvec;
  }

  LogRecord *get_focus(const string &in,
                       const string &axis,
                       const bool fuzzy=false) {
    LogRecord *prec = nullptr;
    if (axis == "message") {
      auto candidate = query_on_message(in, fuzzy);
      if (candidate.empty()) { return nullptr; }
      for (auto &each : candidate) {
        system("clear");
        cout << each->_repr() << endl;
        bool is_correct;
        try {
          is_correct = get_decision("Is this the message you're "
              "looking for?");
        } catch (const std::runtime_error &e) {
          cout << "Unrecognized instruction!" << endl;
          return nullptr;
        }
        if (is_correct) { prec = each->entry; break; }
      } // end for
    } else if (axis == "sender") {
      auto candidate = query_on_sender(in, fuzzy);
      if (candidate.empty()) { return nullptr; }
      for (auto &each : candidate) {
        system("clear");
        cout << each->_repr() << endl;
        bool is_correct;
        try {
          is_correct = get_decision("Is this the message you're "
              "looking for?");
        } catch (const std::runtime_error &e) {
          cout << "Unrecognized instruction!" << endl;
          return nullptr;
        }
        if (is_correct) { prec = each->entry; break; }
      } // end for
    } else { throw std::invalid_argument("Misc::get_focus() Invalid axis!"); }
    // `prec` set at approx.
    // move along
    while (prec) {
      system("clear");
      cout << "======== Current Record ========" << endl;
      cout << "Date: " << prec->get_date() << endl;
      cout << "Sender: " << prec->get_sender() << endl;
      cout << "Message: \n" << prec->get_message() << endl;
      cout << "======== Options ========" << endl;
      cout << "    y - this is the record I'm looking for\n"
           << "    n - next along time\n"
           << "    m - next occurence of this massage\n"
           << "    p - take a peak at all messages in the next 1 second"
           << endl;
      cout << "Your choice: "; cout.flush();
      char oper = '\0';
      cin.get(oper); cin.clear(); cin.ignore(10000, '\n');
      switch(oper) {
        case 'y': case 'Y': { return prec; }
        case 'n': { prec = prec->time_suc; break; }
        case '\n': { break; }
        case 'm': { prec = prec->msg_suc; break; }
        case 'p': {
          vector<LogRecord *> recs = prec->peek(1);
          for (size_t idx = 1, range = recs.size();
               idx != range; ++idx) {
            auto &each = recs[idx];
            cout << each->_repr() << endl;
            cout << "---- MORE ----"; getchar();
          }
          cout << "---- END ----"; getchar();
          cin.clear(); cin.ignore(10000, '\n');
          break;
        }
        default: { break; }
      }
    }   // end while
    cin.clear(); cin.ignore(10000, '\n');
    return nullptr;
  } // get_focus

  Storage &delete_rec(LogRecord *&prec) {
    // find `prec`'s predecessor on all dimentions
    // - find on time axis
    LogRecord *time_pre = nullptr;
    if (this->messages->global_begin == prec) { /* pass */ ; }
    else {  // not first
      time_pre = this->messages->global_begin;
      while (time_pre && time_pre->time_suc != prec) {
        time_pre = time_pre->time_suc;
      }
      if (time_pre == nullptr) {
        throw std::runtime_error("Storage::delete_rec() Time predecessor not "
            "found!");
      }
    }
    // - find on message axis
    // TODO: Fails when message contains cr/lf
    auto &msg_cell = (*this->messages)[prec->message];
    LogRecord *msg_pre = nullptr;
    if (msg_cell.entry == prec) { /* pass */ ; }
    else {
      msg_pre = msg_cell.entry;
      while (msg_pre && msg_pre->msg_suc != prec) {
        msg_pre = msg_pre->msg_suc;
      }
      if (msg_pre == nullptr) {
        throw std::runtime_error("Storage::delete_rec() Message predecessor "
            "not found!");
      }
    }
    // - find on sender axis
    auto &sndr_cell = (*this->senders)[prec->get_sender()];
    LogRecord *sndr_pre = nullptr;
    if (sndr_cell.entry == prec) { /* pass */ ; }
    else {
      sndr_pre = sndr_cell.entry;
      while (sndr_pre && sndr_pre->sender_suc != prec) {
        sndr_pre = sndr_pre->sender_suc;
      }
      if (sndr_pre == nullptr) {
        throw std::runtime_error("Storage::delete_rec() Sender predecessor "
            "not found!");
      }
    }
    // re-chain
    // `prec` is first / not first
    if (time_pre == nullptr) {
      this->messages->global_begin = prec->time_suc;
    } else {
      time_pre->time_suc = prec->time_suc;
    }
    if (msg_pre == nullptr) {
      msg_cell.entry = prec->msg_suc;
    } else {
      msg_pre->msg_suc = prec->msg_suc;
    }
    if (sndr_pre == nullptr) {
      sndr_cell.entry = prec->sender_suc;
    } else {
      sndr_pre->sender_suc = prec->sender_suc;
    }
    // check if `prec` is end of chain
    if (this->messages->global_end == prec) {
      this->messages->global_end = time_pre;
    }
    if (msg_cell.end == prec) {
      msg_cell.end = msg_pre;
    }
    if (sndr_cell.end == prec) {
      sndr_cell.end = sndr_pre;
    }
    // free `prec`
    delete prec;
    prec = nullptr;
    return *this;
  }

};


#endif
