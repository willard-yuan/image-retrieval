cppsugar
========

An library mainly with syntax sugar for C++. It enables easy LINQ-like functional programming for C++, and also provides other utility functions like `Print` and `WriteAllLinesToFile`.

Require python/pylab for the visualization utilities.

Demo usage
=========

You will feel comfortable if from a C#/LINQ or even js/underscore background. Some demo usage is like

```
    #include <cppsugar>
    using namespace func;
    auto vec = Range(0, 2, 20);  // expected to be vector<int>{0, 2, 4, 6, 8, 10, 12, 14, 16, 18}
    vec = Map<int, int>(vec, [](int x) { return x + 1; });  // expected to be vector<int>{1, 3, 5, 7, 9, 11, 13, 15, 17, 19}
    vec = Filter<int>(vec, [](int x) { return (x % 3 == 0); });  // expcted to be vector<int>{3, 9, 15}
```

TODO Items
========

* Chaining usage. Like `auto vec = Func(Range(0, 2, 20)).Map<int>([](int x) { return x + 1; }).Filter([](int x) { return (x % 3 == 0); }).v()`
* Parallel support. Add `.AsParallel()` to turn it parallel with OpenMP!
