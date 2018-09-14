/**
 * Pointer operations
 * Jack Andersen
 */

#include <iostream>
#include <cstdlib>

using namespace std;

/**
 * Driver for running lab
 */
int main(int argc, char** argv)
{
  {
    /* Part 1 */
    int* p1;
    int v1;
    v1 = 42;
    p1 = &v1;
    *p1 = 43;
    cout << v1 << ' ' << p1 << ' ' << &v1 << ' ' << *p1 << endl;

    /* 1: v1 is 43
     * 2: p1 is 0x7ffee4ec0a04 in one particular run
     * 3: &v1 is 0x7ffee4ec0a04 in the same particular run
     * 4: *p1 is 43
     * 5: p1 and &v1 are memory locations (pointers) */

    /* Part 2 */
    p1 = new int;
    cin >> *p1;
    cout << v1 << ' ' << p1 << ' ' << &v1 << ' ' << *p1 << endl;

    /* 1: p1 and *p1 changed from Part 1
     * 2: v1 and &v1 stayed the same */

    //cin >> p1;
    /* 3: The above fails to compile due to iostream not being
     * able to insert values into a pointer */
  }

  {
    /* Part 3 */
#define FAIL_MODE 1
#if FAIL_MODE
    typedef int* IntPtr;
    IntPtr p1, p2;
    int v1;
    p1 = new int;
    p2 = new int;
    cin >> v1;
    p1 = &v1;
    p2 = p1;
    cout << "v1:" << v1 << endl;
    cout << "p1:" << p1 << endl;
    cout << "p2:" << p2 << endl;
    cout << "&v1:" << &v1 << endl;
    cout << "*p1:" << *p1 << endl;
    cout << "*p2:" << *p2 << endl;
    delete p1;
    cout << "v1:" << v1 << endl;
    cout << "p1:" << p1 << endl;
    cout << "p2:" << p2 << endl;
    cout << "&v1:" << &v1 << endl;
    cout << "*p1:" << *p1 << endl;
    cout << "*p2:" << *p2 << endl;
#else
    typedef int* IntPtr;
    IntPtr p1, p2;
    int v1;
    p1 = new int;
    p2 = new int;
    cin >> v1;
    *p1 = v1;
    *p2 = *p1;
    cout << "v1:" << v1 << endl;
    cout << "p1:" << p1 << endl;
    cout << "p2:" << p2 << endl;
    cout << "&v1:" << &v1 << endl;
    cout << "*p1:" << *p1 << endl;
    cout << "*p2:" << *p2 << endl;
    delete p1;
    p1 = nullptr;
    cout << "v1:" << v1 << endl;
    cout << "p1:" << p1 << endl;
    cout << "p2:" << p2 << endl;
    cout << "&v1:" << &v1 << endl;
    if (p1 != nullptr)
      cout << "*p1:" << *p1 << endl;
    cout << "*p2:" << *p2 << endl;
#endif

    /* 1: - v1 is the value entered
     *    - p1,p2,&v1 are 0x7ffeed4219ec in one particular run
     *    - *p1,*p2 are the value entered
     * 2: p2 was indirectly made to point to a value not allocated from the
     *    freestore (just a temporary stack variable)
     *    While running the program with clang's -fsanitize=address, it reports
     *    "Attempting free on address which was not malloc()-ed", otherwise it's a
     *    segmentation fault.
     * 3: By using dereferencing operators to take the value of v1 rather than it's address,
     *    NULL-ing the p1 variable after deleting, and conditionally outputting
     *    it if non-null (or simply not accessing p1 ever again) the program will run
     *    correctly. */
  }

  return 0;
}
