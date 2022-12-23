#include <iostream>

using namespace std;

const unsigned long N = 1000;

// forward declaration
unsigned long * allocate_darray();
void free_darray(unsigned long *);

void fill_array (unsigned long *);
void fill_array2 (unsigned long a[]);

unsigned long get_item(unsigned long a[], unsigned long i);
unsigned long get_item2(unsigned long *p, unsigned long i);

void set_item(unsigned long a[], unsigned long i, unsigned long v);

int main()
{
    unsigned long *data = allocate_darray();
    fill_array2(data);

    cout << get_item(data,1) << endl;

    set_item(data, 1, 500);

    cout << get_item2(data,1) << endl;

    unsigned long *p = data;
    cout << *p << endl;

    for (unsigned long i=0;i<10;i++) {
        cout << *(p++) << ",";
    }
    cout << endl;

    free_darray(data);
    return 0;
}


unsigned long *allocate_darray() {
    return new unsigned long [N];
}

void free_darray(unsigned long *p) {
    if (p) delete [] p;
}

void fill_array (unsigned long *p) {
    for (unsigned long i=0;i<N;i++) {
        *(p+i)=N-i;
        //p[i]=N-i;
    }
}

void fill_array2 (unsigned long a[]) {
    for (unsigned long i=0;i<N;i++) {
        a[i]=N-i;
        //*(a+i)=N-i;
    }
}

unsigned long get_item(unsigned long a[], unsigned long i) {
    return a[i];
}

unsigned long get_item2(unsigned long *p, unsigned long i) {
    return *(p+i);
}

void set_item(unsigned long a[], unsigned long i, unsigned long v) {
    a[i] = v;
}
