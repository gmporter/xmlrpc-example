#define WIN32_LEAN_AND_MEAN  /* required by xmlrpc-c/server_abyss.hpp */

#include <cassert>
#include <stdexcept>
#include <iostream>
#ifdef _WIN32
#  include <windows.h>
#else
#  include <unistd.h>
#endif
#include <vector>

using namespace std;

#include <xmlrpc-c/base.hpp>
#include <xmlrpc-c/registry.hpp>
#include <xmlrpc-c/server_abyss.hpp>

#ifdef _WIN32
  #define SLEEP(seconds) SleepEx(seconds * 1000);
#else
  #define SLEEP(seconds) sleep(seconds);
#endif


class sampleAddMethod : public xmlrpc_c::method {
public:
    sampleAddMethod() {
        // signature and help strings are documentation -- the client
        // can query this information with a system.methodSignature and
        // system.methodHelp RPC.
        this->_signature = "i:ii";
            // method's result and two arguments are integers
        this->_help = "This method adds two integers together";
    }
    void
    execute(xmlrpc_c::paramList const& paramList,
            xmlrpc_c::value *   const  retvalP) {
        
        int const addend(paramList.getInt(0));
        int const adder(paramList.getInt(1));
        
        paramList.verifyEnd(2);

        *retvalP = xmlrpc_c::value_int(addend + adder);
    }
};

class sampleExceptionMethod : public xmlrpc_c::method {
public:
    sampleExceptionMethod() {
        // signature and help strings are documentation -- the client
        // can query this information with a system.methodSignature and
        // system.methodHelp RPC.
        this->_signature = "i:ii";
            // method's result and two arguments are integers
        this->_help = "This method throws an exception if either argument is zero";
    }
    void
    execute(xmlrpc_c::paramList const& paramList,
            xmlrpc_c::value *   const  retvalP) {
        
        int const mul1(paramList.getInt(0));
        int const mul2(paramList.getInt(1));
        paramList.verifyEnd(2);

		if (mul1 == 0 || mul2 == 0) {
			throw (xmlrpc_c::fault("An argument was zero"));
		}
        
        *retvalP = xmlrpc_c::value_int(mul1 * mul2);
    }
};

class sampleStateMethod : public xmlrpc_c::method {
public:
    sampleStateMethod(vector<int> &v) : vec(v) {
        // signature and help strings are documentation -- the client
        // can query this information with a system.methodSignature and
        // system.methodHelp RPC.
        this->_signature = "i:i";
            // method's result is an accumulator
        this->_help = "This method adds the value to an accumulator and returns the result";
    }
    void
    execute(xmlrpc_c::paramList const& paramList,
            xmlrpc_c::value *   const  retvalP) {
        
        int const num1(paramList.getInt(0));
        paramList.verifyEnd(1);

		vec.push_back(num1);

        *retvalP = xmlrpc_c::value_int(vec.size());
    }
protected:
	vector<int> vec;
};

int 
main(int           const, 
     const char ** const) {

	vector<int> v;

    try {
        xmlrpc_c::registry myRegistry;

        xmlrpc_c::methodPtr const sampleAddMethodP(new sampleAddMethod);
        xmlrpc_c::methodPtr const sampleExceptionMethodP(new sampleExceptionMethod);
        xmlrpc_c::methodPtr const sampleStateMethodP(new sampleStateMethod(v));

        myRegistry.addMethod("sample.add", sampleAddMethodP);
        myRegistry.addMethod("sample.mul", sampleExceptionMethodP);
        myRegistry.addMethod("sample.accum", sampleStateMethodP);
        
        xmlrpc_c::serverAbyss myAbyssServer(
            xmlrpc_c::serverAbyss::constrOpt()
            .registryP(&myRegistry)
            .portNumber(8080)
			.logFileName("/tmp/xmlrpc_log"));
        
        myAbyssServer.run();
        // xmlrpc_c::serverAbyss.run() never returns
        assert(false);
    } catch (exception const& e) {
        cerr << "Something failed.  " << e.what() << endl;
    }
    return 0;
}

