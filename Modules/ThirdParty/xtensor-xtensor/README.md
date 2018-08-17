Use git-submodules to get (header-only):
* xtl (base, header-only)
* xtensor (base)
   Depends on:
    - xtl
    - xsimd (library --interface--, optional)
* xsimd
   Depends on:
    - xtl (for complex)
* xtensor-blas

Order:
First  install xtl
Second xsimd
Third  xtensor
Fourth xtensor-blas

Docs:https://xtensor.readthedocs.io/en/latest/container.html

From the docs (August 2018);

Three container classes implementing multi-dimensional arrays are provided: xarray and xtensor and xtensor_fixed.

xarray can be reshaped dynamically to any number of dimensions. It is the container that is the most similar to numpy arrays.
xtensor has a dimension set at compilation time, which enables many optimizations. For example, shapes and strides of xtensor instances are allocated on the stack instead of the heap.
xtensor_fixed has a shape fixed at compile time. This allows even more optimizations, such as allocating the storage for the container on the stack, as well as computing strides and backstrides at compile time, making the allocation of this container extremely cheap.

Let’s use xtensor instead of xarray in the previous example:

```cpp
#include <array>
#include "xtensor/xtensor.hpp"

std::array<size_t, 3> shape = { 3, 2, 4 };
xt::xtensor<double, 3> a(shape);
// whis is equivalent to
// xt::xtensor<double, 3, xt::layout_type::row_major> a(shape);
```

Or when using xtensor_fixed:

```cpp
#include "xtensor/xfixed.hpp"

xt::xtensor_fixed<double, xt::xshape<3, 2, 4>> a();
// or xt::xtensor_fixed<double, xt::xshape<3, 2, 4>, xt::layout_type::row_major>()
```

xarray, xtensor and xtensor_fixed containers are all xexpression and can be involved and mixed in mathematical expressions, assigned to each other etc… They provide an augmented interface compared to other xexpression types:

Each method exposed in xexpression interface has its non-const counterpart exposed by xarray, xtensor and xtensor_fixed.
- reshape() reshapes the container in place, and the global size of the container has to stay the same.
- resize() resizes the container in place, that is, if the global size of the container doesn’t change, no memory allocation occurs.
- strides() returns the strides of the container, used to compute the position of an element in the underlying buffer.
