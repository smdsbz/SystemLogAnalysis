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
    system(string("touch" + filename).c_str());
    system(string("syslog -F bsd > " + filename).c_str());
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
         << "\n"
         << " 31. Remove single log from buffered\n"
         << " 32. Remove message from buffered\n"
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
        for (; axis != "message" && axis != "sender";) {
          cout << "Query range (message/sender): "; cout.flush();
          getline(cin, axis);
        }
        string in;
        cout << "Content: "; cout.flush();
        getline(cin, in);
        bool fuzzy = get_decision("Use fuzzy find?");
        // finished with input
        LogRecord *cursor = storage->get_focus(in, axis, fuzzy);
        if (cursor == nullptr) {
          cout << "Nothing is selected!" << endl;
        } else {
          cout << cursor->get_date() << endl;
          cout << cursor->get_sender() << endl;
          cout << cursor->get_message() << endl;
        }
        show_pause(); break;
      }

      case 4: {
        system("clear");
        cout << "Test Run - FPTree::_first_run()" << endl;
        auto analysis = FPTree(storage);
        analysis.run(10, 1);
        system("clear");
        analysis.show_result(10);
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

