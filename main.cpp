#include <iostream>
using std::cout; using std::cin;

#include <string>
using std::string;

#include <vector>
using std::vector;

#include <stdexcept>

#include "./utils/LogClass/LogClass.h"
#include "./utils/HashTable/HashTable.h"
#include "./utils/StorageGraph/StorageGraph.h"
#include "./utils/FPTree/FPTree.h"
#include "./Misc.h"
using Misc::get_block_input;
#include "./utils/UIGadgets/UIGadgets.h"
using UIGadgets::show_pause; using UIGadgets::get_decision;



int main(int argc, char **argv) {
  system("clear");

  Storage *storage = nullptr;

  if (argc == 2) {  // program started with dumpfile's name
    storage = Misc::load_from_file(argv[1]);
  } else if (argc == 1) {   // called without any arguments
    string filename;
    cout << "Specify path to dump log: ";
    cin >> filename; cin.clear(); cin.ignore(10000, '\n');
    // dump log to file
    system(string("touch " + filename).c_str());
    system(string("syslog -F bsd > " + filename).c_str());
    cout << "Logs successfully dumped from console to " << filename << endl;
    storage = Misc::load_from_file(filename);
  }

  if (storage == nullptr) { return -1; }
  // NOTE: `storage` is promised mounted!
  cout << "Successfully loaded!" << endl;
  show_pause();


  int oper = -1;
  while (oper != 0) {
    // show interface messages
    system("clear");
    cout << endl;
    cout << "    SystemLogAnalysis (OS-X build) (by smdsbz)    \n"
         << "==================================================\n"
         << " 11. Query on message contents\n"
         << " 12. Query on sender\n"
         << " 13. General query\n"
         << "\n"
         << " 21. Insert new log manually\n"
         << "- - - - - - - - - - - - - - - - - - - - - - - - -\n"
         << " 4 . Run analysis\n"
         << "- - - - - - - - - - - - - - - - - - - - - - - - -\n"
         << " 0 . Exit program\n"
         << "==================================================\n"
         << "Choose your operation: ";
    cout.flush();
    cin >> oper;    // get oper from user
    cin.clear(); cin.ignore(10000, '\n');   // get rid of '\n's

    switch (oper) {

      case 11: {
        string in_msg; string fuzzy_ind; bool use_fuzzy;
        try {
          use_fuzzy = get_decision("Use fuzzy find?");
        } catch (const std::runtime_error &e) { // unrecognized symbol
          cout << "Unrecognized symbol!" << endl;
          show_pause(); break;
        }
        cout << "Message content: "; cout.flush();
        getline(cin, in_msg);
        auto search_result = storage->query_on_message(in_msg, use_fuzzy);
        Misc::show_query_result(search_result);
        show_pause(); break;
      }

      case 12: {
        string in_sender;
        cout << "Sender name: "; cout.flush();
        getline(cin, in_sender);
        auto search_result = storage->query_on_sender(in_sender);
        Misc::show_query_result(search_result);
        show_pause(); break;
      }

      case 13: {
        string axis;
        for (; axis != "message"
                && axis != "sender"
                && axis != "time";) {
          cout << "Query range (message/sender): "; cout.flush();
          getline(cin, axis);
        }
        string in;
        cout << "Content: "; cout.flush();
        getline(cin, in);
        bool fuzzy = false;
        if (axis != "time") {
          fuzzy = get_decision("Use fuzzy find?");
        }
        // finished with input
        LogRecord *cursor = storage->get_focus(in, axis, fuzzy);
        system("clear");
        if (cursor == nullptr) {
          cout << "Nothing is selected!" << endl;
        } else {
          cout << "======== Current Record ========" << endl;
          cout << "Date: " << cursor->get_date() << endl;
          cout << "Host: " << cursor->get_host() << endl;
          cout << "Sender: " << cursor->get_sender() << endl;
          cout << "Message: \n" << cursor->get_message() << endl;
          cout << "======== Options ========" << endl;
          cout << "    d - delete this record" << endl;
          cout << "    a - add new log after this one" << endl;
          cout << "    0 - cancel" << endl;
          cout << "Your choice: "; cout.flush();
          char opt = '\0';
          cin.get(opt); cin.clear(); cin.ignore(10000, '\n');
          switch (opt) {
            case 'd': {
              storage->delete_rec(cursor);
              cout << "Successfully deleted record!" << endl;
              break;
            }
            case 'a': {
              auto log_message = get_block_input(/*end_flag=*/"EOF",
                                                 /*with_tab=*/true);
              try {
                storage->add_after_rec(cursor, log_message);
                cout << "Successfully added record!" << endl;
              } catch (const std::runtime_error &e) {
                cout << "Failed adding record!\n"
                     << "Do spell checking before hitting <Enter>!" << endl;
              }
              break;
            }
            case '0': case '\n': { break; }
            default: { break; }
          }
        }   // endif
        show_pause(); break;
      }

      case 4: {
        system("clear");
        auto analysis = FPTree(storage);
        analysis.run(/*min_freq=*/10,
                     /*max_delay=*/1);
        cout << "Finished analyzing!" << endl;
        show_pause();
        system("clear");
        analysis.show_result(/*valve_freq=*/10);
        show_pause(); break;
      }

      case 0: { break; }
      default: { break; }

    }

  } // reploop

  system("clear");
  cout << "Exiting..." << endl;
  delete storage;
  return 0;
}

