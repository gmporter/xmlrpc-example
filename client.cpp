#include <cstdlib>
#include <string>
#include <iostream>

using namespace std;

#include <xmlrpc-c/girerr.hpp>
#include <xmlrpc-c/base.hpp>
#include <xmlrpc-c/client_simple.hpp>

int
main(int argc, char **) {

    if (argc-1 > 0) {
        cerr << "This program has no arguments" << endl;
        exit(1);
    }

    try {
        string const serverUrl("http://localhost:8080/RPC2");

        xmlrpc_c::clientSimple myClient;
        
		// Test the 'add' call
        xmlrpc_c::value addresult;
        myClient.call(serverUrl, "sample.add", "ii", &addresult, 3, 4);
		int const sum((xmlrpc_c::value_int(addresult)));
		cout << "3 + 4 = " << sum << endl;

		// Test multiplication without an exception
        xmlrpc_c::value mulresult;
		myClient.call(serverUrl, "sample.mul", "ii", &mulresult, 2, 3);
		int const multresult((xmlrpc_c::value_int(mulresult)));
		cout << "2 * 3 = " << multresult << endl;

		// Test multiplication with an exception
        xmlrpc_c::value mulexceptionresult;
		myClient.call(serverUrl, "sample.mul", "ii", &mulexceptionresult, 0, 3);
		int const exceptionresult((xmlrpc_c::value_int(mulexceptionresult)));
		cout << "2 * 3 = " << exceptionresult << endl;

    } catch (exception const& e) {
        cerr << "Client threw error: " << e.what() << endl;
    } catch (...) {
        cerr << "Client threw unexpected error." << endl;
    }

    return 0;
}
