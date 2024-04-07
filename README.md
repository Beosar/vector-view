# C++ vector view
Slice of a vector with support for emplace and erase.

Usage:

```
std::vector<int> v = {0,1,2,3,4,5};
vector_view<int> vv(v, v.begin(), v.begin() + 3);
const_vector_view<int> cvv(v.begin(), v.begin() + 4);
```

Use `emplace` or `emplace_back` just like the `std::vector` equivalent:
```
vv.emplace_back(6); //v is now 0,1,2,6,3,4,5 and vv is 0,1,2,6
```

`erase` supports both single iterators and a begin and end iterator. Use `pop_back` to remove the last element.
```
vv.erase(vv.begin()+1); //v is now 0,2,6,3,4,5 and vv is 0,2,6
```

Iterate over `vector_view` just like over a `std::vector`:
```
for(auto& i : vv) i += 100; //v is now 100,102,106,3,4,5 and vv is 100,102,106
```
