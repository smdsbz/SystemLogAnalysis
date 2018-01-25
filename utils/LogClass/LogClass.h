#ifndef LOGCLASS_H_
#define LOGCLASS_H_


class LogDate {
  public:
    struct {
      unsigned short mon : 4;   // 1 to 12
      unsigned short dat : 5;   // 1 to 31
      unsigned short hor : 5;   // 0 to 23
      unsigned short min : 6;   // 0 to 59
      unsigned short sec : 6;   // 0 to 59
      unsigned long  _   : 6;
    } time;

  public:
    LogDate();
    LogDate(string re_mathed_string);
    void _init_to_zero();
    string str();
};


class LogMessage {
  public:
    // LogDate   date;
    string    host;
    string    sender;
    // string    thread;
    string    message;

  public:
    LogMessage();
    LogMessage(string raw_input_str);
    void append_msg(string more_message_str);
    uint32_t hash();

};

#endif
