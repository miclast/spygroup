
//		Copyright (R) 2011 SfinxSoft, All rights reserved
//
// Web:		http://sfinxsoft.com
// Email:	info [at] sfinxsoft [dot] com
//

#include <astxx/manager.h>
#include <astxx/manager/action/agi.h>
#include <vector>
#include <string>
#include <iostream>
#include <mysql++.h>

namespace manager = astxx::manager;
namespace action = astxx::manager::action;
manager::connection *connection;
static int debug = 0;
using namespace std;

void set_var(string ch, string var, string val)
{
 action::setvar v(ch, var, val);
 v(*connection);
}

void verbose(string m)
{
 time_t t;
 char ascii_time[128];
 t = time(0);
 strcpy(ascii_time, ctime(&t));
 ascii_time[strlen(ascii_time) - 1] = 0;
 cerr << ascii_time << ": " << m << endl;
}

mysqlpp::Connection *con;

string get_group(string &cid)
{
 string group;
 mysqlpp::Query query = con->query("select id from spygroups where extension = " + cid);
 mysqlpp::StoreQueryResult res = query.store();
 if (res.num_rows())
   group = (res[0]["id"]).c_str();
 return group;
}

void process_event(astxx::manager::message::event e)
{
   if (e["Event"] == "Bridge" || e["Event"] == "Link") {
     string ch1 = e["Channel1"];
     string ch2 = e["Channel2"];
     string cid1 = e["CallerID1"];
     string cid2 = e["CallerID2"];
     // check both cids for SPYGROUP
     string spygroup = get_group(cid1);
     if (spygroup.size()) {
       verbose("setting group " + spygroup + " on " + ch1 + " for " + cid1);
       set_var(ch1, "SPYGROUP", spygroup);
     }
     spygroup = get_group(cid2);
     if (spygroup.size()) {
       verbose("setting group " + spygroup + " on " + ch2 + " for " + cid2);
       set_var(ch2, "SPYGROUP", spygroup);
     }
   }
}

int main(int argc, char **argv) {

   if (argc > 1)
     debug = 1;

   string ami_host = "localhost";
   string ami_name = "ami7";
   string ami_pass = "ami7_pass";
   
   try {
      con = new mysqlpp::Connection("ast", "localhost", "ast_user", "ast_pass");
      connection = new manager::connection(ami_host);
      cerr << "Connected to " << connection->name() << " v" <<
         connection->version() << endl;

      boost::signals::scoped_connection c =
         connection->register_event("", process_event);
      action::login login(ami_name, ami_pass);
      login(*connection);

      for (;;) {
         connection->wait_event();
         connection->pump_messages();
         connection->process_events();
      }
      return 0;
   }
   catch (const mysqlpp::BadQuery& e) {
      // Something went wrong with the SQL query.
      cerr << "Query failed: " << e.what();
      exit(-1);
  }
   catch (const mysqlpp::Exception& er) {
      // Catch-all for any other MySQL++ exceptions
      cerr << "Error: " << er.what();
      exit(-1);
  }
   catch (const manager::action::channel_not_found &c) {
      cerr << "main: channel not found " << endl;
   }
   catch (const exception& e) {
      cerr << "main: exception: " << e.what() << endl;
   }
   return 1;
}
