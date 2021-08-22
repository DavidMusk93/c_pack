#ifndef C_PLAYGROUND_MACRO_H
#define C_PLAYGROUND_MACRO_H

#define __str(x) #x
#define STR(x) __str(x)
#define CONCAT(x,y) x##y

#define MAIN() int main()
#define MAIN_EX(argc,argv) int main(int argc,char*argv[])

#define __ATTR(x) __attribute__((x))
#define __scoped_guard(fn) __ATTR(__cleanup__(fn))
#define __ctor __ATTR(constructor)
#define __dtor __ATTR(destructor)
#define __packed __ATTR(packed)

#define SWAP(x,y,t) t=x,x=y,y=t

#endif //C_PLAYGROUND_MACRO_H
