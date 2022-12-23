#include <iostream>
#include <dlfcn.h>

using namespace std;

int main()
{
     void* handle = dlopen("libdlib.so", RTLD_LAZY);

     if (!handle) {
             cerr << "Cannot open library: " << dlerror() << '\n';
             return 1;
     }

     // load the symbol
     typedef unsigned (*square_t)(unsigned);

     // reset errors
     dlerror();
     // old style casting
     //square_t function = (square_t) dlsym(handle, "square");
     // new style casting
     square_t function = reinterpret_cast <square_t>(dlsym(handle, "square"));

     const char *dlsym_error = dlerror();
     if (dlsym_error) {
         cerr << "Cannot load symbol 'square': " << dlsym_error << endl;
         dlclose(handle);
         return 1;
     }

    cout << "Hello World! - " << function(9) << endl;
    dlclose(handle);
 }
