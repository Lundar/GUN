#ifndef MACROS_H_INCLUDED
#define MACROS_H_INCLUDED

#define for_map(x , xx , y , z)  for(x,xx::iterator y = z.begin(); y != z.end(); y++)
#define for_each(x , y , z)  for(x::iterator y = z.begin(); y != z.end(); y++)

//typedef unsigned int uint;

#endif // MACROS_H_INCLUDED
