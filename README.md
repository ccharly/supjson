supjson
=========

1. [Philosophy](#philosophy)
2. [Getting started](#getting-started)

##<a id="philosophy"></a>Philosophy
The library is designed to be stack only objects. TODO


##<a id="getting-started"></a>Getting started
Here are the types defined by the library:
```cpp
// A little namespace alias
namespace sj = supjson;

lj::null;
lj::array;
lj::object;
lj::number;
lj::boolean;

lj::value; // is an opaque value that can be any type listed above
```
