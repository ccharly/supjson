lyza.json
=========

1. [Philosophy](#philosophy)
2. [Getting started](#getting-started)

##<a id="philosophy"></a>Philosophy
The library is designed to be stack only objects. TODO


##<a id="getting-started"></a>Getting started
Here are the types defined by the library:
```cpp
// A little namespace alias
namespace lj = lyza::json;

lj::null;
lj::array;
lj::object;
lj::number;
lj::boolean;
```

And here a little example that shows how to use the library:
```cpp
...
// A little namespace alias
namespace lj = lyza::json;

// By default, a value is initilized to null
lj::value val;

val = 10;
val = 10.2;
val = std::string("a json string");

// A JSON array is a vector of value
lj::array a;

a.push_back(true);
a.push_back(false);
a.push_back(10);
a.push_back(std::string("a string in an array"));

// A JSON object
lj::object o;

o["key"] = val;
o["json_array"] = a;

// Nested objects
lj::object oo;

oo["json_bool"] = true;
o["nested"] = oo;

// Print JSON
std::cout << lj::to_string(o) << std::endl;
...
```

You can also read a JSON from a file or a string:
```cpp
...
// A little namespace alias
namespace lj = lyza::json;

// Read a JSON file
lj::producer p = lj::producer::from_file("your-file.json");

// Or read JSON from a string like this:
// lj::producer p = lj::producer::from_string("{\"key\": \"value\"}");

if (!p.good()) // check if the producer is useable
	return 1;

// Get JSON value from the producer
lj::value val = lj::parser::parse(p);

// Print JSON
std::cout << lj::to_string(val) << std::endl;
...
```

TODO
