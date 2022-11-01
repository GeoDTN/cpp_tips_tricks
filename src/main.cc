#include <iostream>
// Difference between class and struct
// the default access specifier on a struct is public; on a class it’s private

// the default inheritance is public for structs; private for classes.

// for modern C++ there is also a third difference: the way brace initialization is handled.

// for a structure, brace initialization performs a direct initialization of its members

struct POD {
    int i;
    int j;
};


int main()
{
    POD pod1 { 1, 2 }  // pod1.i => 1
                       // pod1.j => 2  
}

// for class types, brace initialization results in a call to a constructor.

class ADT {
public:
    ADT(int x, int y);

    void op();

private:
    int a;
    int b;
};


int main()
{
    ADT adt1 { 1, 2 };   // ADT::ADT(1, 2);
}

// there’s nothing to stop you adding a constructor to a struct. If you do, the braced initialization list becomes a call to a constructor.

struct POD {
    int i;
    int j;

    POD(int x, int y);
};


int main()
{
    POD pod1 { 1, 2 };    // POD::POD(1, 2);
}

// there are some odd corner cases, that might throw you:

class ADT {
public:
    // NOTE - public attributes (Yeah, I know...)
    //
    int a;
    int b;

    // Explicitly deleted constructors
    //
    ADT(int, int) = delete;
    ADT() = default;

    void op() { };
private:
};


int main()
{
    ADT adt1 { };        // OK!?
    ADT adt2 { 1, 2 };   // OK!? What gives?...
}

// even though we’ve explicitly deleted the two constructors, this code compiles just fine. The reason this works (maybe against your expectations) is that both objects are being direct initialized – that is, the constructors are not being called. The compiler can do this as the attributes are declared as public.

// we can ‘break’ this code by actually defining one (or both) of the constructors explicitly

class ADT {
public:
    int a;
    int b;

    ADT(int, int) = delete;
    ADT() { /* ctor definition... */ };

    void op() { };
private:
};


int main()
{
    ADT adt1 { };       // OK
    ADT adt2 { 1, 2 };  // ERROR - ctor is deleted (as expected)
}

// the compiler treats user-defined types with only public attributes as an aggregate type. It will attempt to direct initialize the members unless an appropriate constructor is defined.
// by extension, if I declare any private attributes aggregate initialization can no longer be applied (you can’t access the private member to initialize it); therefore, the braced initialization list must be used to call a constructor.

class ADT {
public:
    int a;
    int b;

    ADT(int, int) = delete;
    ADT() = delete;

    void op() { };
private:
    int c; // <= Private data member
};


int main()
{
    ADT adt1 { };       // ERROR - Default ctor is deleted
    ADT adt2 { 1, 2 };  // ERROR - Non-default ctor is deleted
}

