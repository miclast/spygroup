
//		Copyright (R) 2011 SfinxSoft, All rights reserved
//
// Web:		http://sfinxsoft.com
// Email:	info [at] sfinxsoft [dot] com
//

#include <map>
#include <astxx/agi.h>
#include <sstream>
#include <string.h>
#include <stdlib.h>
#include <mysql++.h>

using namespace std;
using namespace astxx;

int debug;

int main(int argc, char** argv)
{
 debug = (argc > 1) ? 1 : 0;

   try {
      astxx::agi& agi = astxx::agi::instance();

     agi.answer();
     // get the PIN
     string pin;
     while (!pin.size())
      pin = agi.get_data("en/agent-pass", 5000, 10);
     mysqlpp::Connection con("ast", "localhost", "ast_user", "ast_pass");
     mysqlpp::Query query = con.query("select id from group_pass where pass = " + pin);
     mysqlpp::StoreQueryResult res = query.store();
     if (!res.num_rows())
      agi.verbose("No SPYGROUP defined for " + pin);
     else {      
      agi.verbose(string("Select SPYGROUP=") + (res[0]["id"]).c_str() + string(" for monitoring"));
      string spy_opts = string("SIP,g(") + string((res[0]["id"]).c_str()) + string(")");
      agi.verbose("executing " + spy_opts);
      agi.exec("ChanSpy", spy_opts);
     }
  } // try
   catch (astxx::agi::error &e) {
      stringstream ss;
      ss << "An error occured in the agi script:\n   " << e.what();
      astxx::agi::instance().verbose(ss.str());
      exit(-1);
  }
   catch (const mysqlpp::BadQuery& e) {
      stringstream ss;
      // Something went wrong with the SQL query.
      ss << "Query failed: " << e.what();
      astxx::agi::instance().verbose(ss.str());
      exit(-1);
  }
   catch (const mysqlpp::Exception& er) {
      stringstream ss;
      // Catch-all for any other MySQL++ exceptions
      ss << "Error: " << er.what();
      astxx::agi::instance().verbose(ss.str());
      exit(-1);
  }  
   return 0;
}
