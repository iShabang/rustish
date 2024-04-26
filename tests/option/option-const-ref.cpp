#include <catch2/catch_test_macros.hpp>

#include "../Option.hpp"

using namespace option;

TEST_CASE("Initialize Option with None structure", "[const-ref]") {
  Option<const int &> opt = None();
  REQUIRE(opt.is_none());
  REQUIRE(!opt.is_some());
}

TEST_CASE("Option default constructor", "[const-ref]") {
  Option<const int &> opt;
  REQUIRE(opt.is_none());
  REQUIRE(!opt.is_some());
}

TEST_CASE("Initialize Option with Some function", "[const-ref]") {
  const int value = 5;
  Option<const int &> opt = Some(value);
  REQUIRE(opt.is_some());
  REQUIRE(!opt.is_none());
  REQUIRE(&opt.unwrap_unchecked() == &value);
}

TEST_CASE("is_some_and returns true on a full Option with a true predicate",
          "[const-ref]") {
  const int value = 5;
  const int *actual = nullptr;
  REQUIRE(Some(value).is_some_and([&](const int &val) {
    actual = &val;
    return true;
  }));
  REQUIRE(actual == &value);
}

TEST_CASE("is_some_and returns false on a full Option with a false predicate",
          "[const-ref]") {
  const int value = 5;
  REQUIRE(!Some(value).is_some_and([](const int &val) { return false; }));
}

TEST_CASE("is_some_and returns false on an empty Option with a true predicate",
          "[const-ref]") {
  REQUIRE(!Option<const int &>().is_some_and([](const int &val) { return true; }));
}

TEST_CASE("is_some_and returns false on an empty Option with a false predicate",
          "[const-ref]") {
  REQUIRE(!Option<const int &>().is_some_and([](const int &val) { return false; }));
}

TEST_CASE("as_ref returns an Option containing a reference to the same value",
          "[const-ref]") {
  const int value = 5;
  Option<const int &> a = Some(value);
  Option<const int &> b = a.as_ref();
  REQUIRE(b.is_some());
  REQUIRE(!b.is_none());
  REQUIRE(b.unwrap_unchecked() == 5);
}

TEST_CASE("as_ref returns an empty Option from an empty Option", "[const-ref]") {
  Option<const int &> a = None();
  Option<const int &> b = a.as_ref();
  REQUIRE(b.is_none());
  REQUIRE(!b.is_some());
}

TEST_CASE("as_mut returns an Option containing a reference to the same value",
          "[const-ref]") {
  const int value = 5;
  Option<const int &> a = Some(value);
  Option<const int &> b = a.as_mut();
  REQUIRE(b.is_some());
  REQUIRE(!b.is_none());
  REQUIRE(b.unwrap_unchecked() == 5);
}

TEST_CASE("as_mut returns an empty Option from an empty Option", "[const-ref]") {
  Option<const int &> a = None();
  Option<const int &> b = a.as_mut();
  REQUIRE(b.is_none());
  REQUIRE(!b.is_some());
}

TEST_CASE("expect returns the value on a full Option", "[const-ref]") {
  const int value = 5;
  REQUIRE(&Some(value).expect("Something is wrong with Option::expect") ==
          &value);
}

TEST_CASE("unwrap returns the value on a full Option", "[const-ref]") {
  const int value = 5;
  REQUIRE(&Some(value).unwrap() == &value);
}

TEST_CASE("unwrap_or returns the value on a full Option", "[const-ref]") {
  const int value = 5;
  int other = 6;
  REQUIRE(&Some(value).unwrap_or(other) == &value);
}

TEST_CASE("unwrap_or returns the other value on an empty Option", "[const-ref]") {
  int other = 6;
  REQUIRE(&Option<const int &>().unwrap_or(other) == &other);
}

TEST_CASE("unwrap_or_else returns other value on a full Option", "[const-ref]") {
  const int value = 5;
  int other = 6;
  REQUIRE(&Some(value).unwrap_or_else([&]() -> int & { return other; }) ==
          &value);
}

TEST_CASE("unwrap_or_else returns the other value on an empty Option",
          "[const-ref]") {
  int other = 6;
  REQUIRE(&Option<const int &>().unwrap_or_else([&]() -> int & { return other; }) ==
          &other);
}

TEST_CASE("unwrap_unchecked returns the value on a full Option", "[const-ref]") {
  const int value = 5;
  REQUIRE(&Option<const int &>(value).unwrap_unchecked() == &value);
}

TEST_CASE("map returns a new Option", "[const-ref]") {
  const int value = 5;
  Option<const int &> a = Some(value);

  SECTION("new value type") {
    Option<double> b = a.map([](const int &val) { return 2.0; });
    REQUIRE(b.is_some());
    REQUIRE(!b.is_none());
    REQUIRE(b.unwrap_unchecked() == 2.0);
  }

  SECTION("new reference type") {
    double newVal = 2.0;
    Option<double &> b = a.map([&](const int &val) -> double & { return newVal; });
    REQUIRE(b.is_some());
    REQUIRE(!b.is_none());
    REQUIRE(&b.unwrap_unchecked() == &newVal);
  }

  SECTION("new const reference type") {
    double newVal = 2.0;
    Option<const double &> b =
        a.map([&](const int &val) -> const double & { return newVal; });
    REQUIRE(b.is_some());
    REQUIRE(!b.is_none());
    REQUIRE(&b.unwrap_unchecked() == &newVal);
  }
}

TEST_CASE("map returns another empty Option", "[const-ref]") {
  Option<int> a = None();

  SECTION("new value type") {
    Option<double> b = a.map([](const int &&val) { return 2.0; });
    REQUIRE(b.is_none());
    REQUIRE(!b.is_some());
  }

  SECTION("new reference type") {
    double newVal = 2.0;
    Option<double &> b = a.map([&](const int &&val) -> double & { return newVal; });
    REQUIRE(b.is_none());
    REQUIRE(!b.is_some());
  }

  SECTION("new const reference type") {
    double newVal = 2.0;
    Option<const double &> b =
        a.map([&](const int &&val) -> const double & { return newVal; });
    REQUIRE(b.is_none());
    REQUIRE(!b.is_some());
  }
}

TEST_CASE("map receives the correct value", "[const-ref]") {
  const int *actual = nullptr;
  const int value = 5;
  Some(value).map([&](const int &val) {
    actual = &val;
    return 2.0;
  });
  REQUIRE(actual == &value);
}

TEST_CASE("inspect returns a full Option", "[const-ref]") {
  const int *actual = nullptr;
  const int value = 5;
  Option<const int &> a = Some(value).inspect([&](const int &val) { actual = &val; });
  REQUIRE(a.is_some());
  REQUIRE(!a.is_none());
  REQUIRE(&a.unwrap_unchecked() == &value);
  REQUIRE(actual == &value);
}

TEST_CASE("inspect returns an empty option", "[const-ref]") {
  const int *actual = nullptr;
  Option<const int &> a =
      Option<const int &>().inspect([&](const int &val) { actual = &val; });
  REQUIRE(a.is_none());
  REQUIRE(!a.is_some());
  REQUIRE(actual == nullptr);
}

TEST_CASE("map_or computes a new value", "[const-ref]") {
  const int *actual = nullptr;
  const int value = 5;
  Option<const int &> a = Some(value);

  SECTION("new value type") {
    Option<double> b = a.map_or(1.0, [&](const int &val) {
      actual = &val;
      return 2.0;
    });
    REQUIRE(b.is_some());
    REQUIRE(!b.is_none());
    REQUIRE(b.unwrap_unchecked() == 2.0);
    REQUIRE(actual == &value);
  }

  SECTION("new reference type") {
    double goodVal = 2.0;
    double defVal = 1.0;
    Option<double &> b = a.map_or(defVal, [&](const int &val) -> double & {
      actual = &val;
      return goodVal;
    });
    REQUIRE(b.is_some());
    REQUIRE(!b.is_none());
    REQUIRE(&b.unwrap_unchecked() == &goodVal);
    REQUIRE(actual == &value);
  }

  SECTION("new const reference type") {
    double goodVal = 2.0;
    const double defVal = 1.0;
    Option<const double &> b =
        a.map_or(defVal, [&](const int &val) -> const double & {
          actual = &val;
          return goodVal;
        });
    REQUIRE(b.is_some());
    REQUIRE(!b.is_none());
    REQUIRE(&b.unwrap_unchecked() == &goodVal);
    REQUIRE(actual == &value);
  }
}

TEST_CASE("map_or returns the default value", "[const-ref]") {
  const int *actual = nullptr;
  Option<const int &> a = None();

  SECTION("new value type") {
    Option<double> b = a.map_or(1.0, [&](const int &val) {
      actual = &val;
      return 2.0;
    });
    REQUIRE(b.is_some());
    REQUIRE(!b.is_none());
    REQUIRE(b.unwrap_unchecked() == 1.0);
    REQUIRE(actual == nullptr);
  }

  SECTION("new reference type") {
    double newVal = 2.0;
    double defVal = 1.0;
    Option<double &> b = a.map_or(defVal, [&](const int &val) -> double & {
      actual = &val;
      return newVal;
    });
    REQUIRE(b.is_some());
    REQUIRE(!b.is_none());
    REQUIRE(&b.unwrap_unchecked() == &defVal);
    REQUIRE(actual == nullptr);
  }

  SECTION("new const reference type") {
    double newVal = 2.0;
    const double defVal = 1.0;
    Option<const double &> b =
        a.map_or(defVal, [&](const int &val) -> const double & {
          actual = &val;
          return newVal;
        });
    REQUIRE(b.is_some());
    REQUIRE(!b.is_none());
    REQUIRE(&b.unwrap_unchecked() == &defVal);
    REQUIRE(actual == nullptr);
  }
}

TEST_CASE("map_or_else computes the default value", "[const-ref]") {
  const int *actual = nullptr;
  Option<const int &> empty = None();

  SECTION("new value type") {
    Option<double> a = empty.map_or_else([]() { return 1.0; },
                                         [&](const int &val) {
                                           actual = &val;
                                           return 2.0;
                                         });
    REQUIRE(a.is_some());
    REQUIRE(!a.is_none());
    REQUIRE(a.unwrap_unchecked() == 1.0);
    REQUIRE(actual == nullptr);
  }

  SECTION("new reference type") {
    double newVal = 2.0;
    double defVal = 1.0;
    Option<double &> a = empty.map_or_else([&]() -> double & { return defVal; },
                                           [&](const int &val) -> double & {
                                             actual = &val;
                                             return newVal;
                                           });
    REQUIRE(a.is_some());
    REQUIRE(!a.is_none());
    REQUIRE(&a.unwrap_unchecked() == &defVal);
    REQUIRE(actual == nullptr);
  }

  SECTION("new const reference type") {
    double newVal = 2.0;
    double defVal = 1.0;
    Option<const double &> a =
        empty.map_or_else([&]() -> const double & { return defVal; },
                          [&](const int &val) -> const double & {
                            actual = &val;
                            return newVal;
                          });
    REQUIRE(a.is_some());
    REQUIRE(!a.is_none());
    REQUIRE(&a.unwrap_unchecked() == &defVal);
    REQUIRE(actual == nullptr);
  }
}

TEST_CASE("map_or_else computes a new value", "[const-ref]") {
  const int *actual = nullptr;
  const int value = 5;
  Option<const int &> opt = Some(value);

  SECTION("new value type") {
    Option<double> a = opt.map_or_else([]() { return 1.0; },
                                       [&](const int &val) {
                                         actual = &val;
                                         return 2.0;
                                       });
    REQUIRE(a.is_some());
    REQUIRE(!a.is_none());
    REQUIRE(a.unwrap_unchecked() == 2.0);
    REQUIRE(actual == &value);
  }

  SECTION("new reference type") {
    double newVal = 2.0;
    double defVal = 1.0;
    Option<double &> a = opt.map_or_else([&]() -> double & { return defVal; },
                                         [&](const int &val) -> double & {
                                           actual = &val;
                                           return newVal;
                                         });
    REQUIRE(a.is_some());
    REQUIRE(!a.is_none());
    REQUIRE(&a.unwrap_unchecked() == &newVal);
    REQUIRE(actual == &value);
  }

  SECTION("new const reference type") {
    double newVal = 2.0;
    double defVal = 1.0;
    Option<const double &> a =
        opt.map_or_else([&]() -> const double & { return defVal; },
                        [&](const int &val) -> const double & {
                          actual = &val;
                          return newVal;
                        });
    REQUIRE(a.is_some());
    REQUIRE(!a.is_none());
    REQUIRE(&a.unwrap_unchecked() == &newVal);
    REQUIRE(actual == &value);
  }
}

TEST_CASE("and_ returns the new Option", "[const-ref]") {

  const int value = 5;
  Option<const int &> a = Some(value);
  SECTION("same type") {
    const int newVal = 6;
    Option<const int &> b = a.and_(Some(newVal));
    REQUIRE(b.is_some());
    REQUIRE(!b.is_none());
    REQUIRE(&b.unwrap_unchecked() == &newVal);
  }

  SECTION("new value type") {
    Option<double> b = a.and_(Some(2.0));
    REQUIRE(b.is_some());
    REQUIRE(!b.is_none());
    REQUIRE(b.unwrap_unchecked() == 2.0);
  }

  SECTION("new reference type") {
    double newVal = 2.0;
    Option<double &> b = a.and_(Some(newVal));
    REQUIRE(b.is_some());
    REQUIRE(!b.is_none());
    REQUIRE(&b.unwrap_unchecked() == &newVal);
  }

  SECTION("new const reference type") {
    const double newVal = 2.0;
    Option<const double &> b = a.and_(Some(newVal));
    REQUIRE(b.is_some());
    REQUIRE(!b.is_none());
    REQUIRE(&b.unwrap_unchecked() == &newVal);
  }
}

TEST_CASE("and_ returns an empty Option", "[const-ref]") {
  Option<double> a = Option<const int &>().and_(Some(2.0));
  REQUIRE(a.is_none());
  REQUIRE(!a.is_some());
}

TEST_CASE("and_then returns an empty Option", "[const-ref]") {
  Option<double> a =
      Option<const int &>().and_then([](const int &val) { return Some(2.0); });
  REQUIRE(a.is_none());
  REQUIRE(!a.is_some());
}

TEST_CASE("and_then returns a new full Option", "[const-ref]") {
  const int *actual = nullptr;
  const int value = 5;
  Option<const int &> a = Some(value);

  SECTION("same type") {
    const int newVal = 6;
    Option<const int &> b = a.and_then([&](const int &val) {
      actual = &val;
      return Some(newVal);
    });
    REQUIRE(b.is_some());
    REQUIRE(!b.is_none());
    REQUIRE(&b.unwrap_unchecked() == &newVal);
    REQUIRE(actual == &value);
  }

  SECTION("new value type") {
    Option<double> b = a.and_then([&](const int &val) {
      actual = &val;
      return Some(2.0);
    });
    REQUIRE(b.is_some());
    REQUIRE(!b.is_none());
    REQUIRE(b.unwrap_unchecked() == 2.0);
    REQUIRE(actual == &value);
  }

  SECTION("new reference type") {
    double newVal = 2.0;
    Option<double &> b = a.and_then([&](const int &val) {
      actual = &val;
      return Some(newVal);
    });
    REQUIRE(b.is_some());
    REQUIRE(!b.is_none());
    REQUIRE(&b.unwrap_unchecked() == &newVal);
    REQUIRE(actual == &value);
  }

  SECTION("new const reference type") {
    const double newVal = 2.0;
    Option<const double &> b = a.and_then([&](const int &val) {
      actual = &val;
      return Some(newVal);
    });
    REQUIRE(b.is_some());
    REQUIRE(!b.is_none());
    REQUIRE(&b.unwrap_unchecked() == &newVal);
    REQUIRE(actual == &value);
  }
}

TEST_CASE("and_then returns a new empty Option", "[const-ref]") {
  const int *actual = nullptr;
  const int value = 5;
  Option<double> a = Some(value).and_then([&](const int &val) {
    actual = &val;
    return Option<double>();
  });
  REQUIRE(a.is_none());
  REQUIRE(!a.is_some());
  REQUIRE(actual == &value);
}

TEST_CASE("filter returns the same Option when given a true predicate",
          "[const-ref]") {
  const int *actual = nullptr;
  const int value = 5;
  Option<const int &> a = Some(value).filter([&](const int &val) {
    actual = &val;
    return true;
  });
  REQUIRE(a.is_some());
  REQUIRE(!a.is_none());
  REQUIRE(&a.unwrap_unchecked() == &value);
  REQUIRE(actual == &value);
}

TEST_CASE("filter returns an empty Option when given a false predicate",
          "[const-ref]") {
  const int *actual = nullptr;
  const int value = 5;
  Option<const int &> a = Some(value).filter([&](const int &val) {
    actual = &val;
    return false;
  });
  REQUIRE(a.is_none());
  REQUIRE(!a.is_some());
  REQUIRE(actual == &value);
}

TEST_CASE("filter returns an empty Option when empty", "[const-ref]") {
  const int *actual = nullptr;
  Option<const int &> a = Option<const int &>().filter([&](const int &val) {
    actual = &val;
    return true;
  });
  REQUIRE(a.is_none());
  REQUIRE(!a.is_some());
  REQUIRE(actual == nullptr);
}

TEST_CASE("or_ returns the same Option", "[const-ref]") {
  const int value = 5;
  const int other = 4;
  Option<const int &> a = Some(value).or_(Some(other));
  REQUIRE(a.is_some());
  REQUIRE(!a.is_none());
  REQUIRE(&a.unwrap_unchecked() == &value);
}

TEST_CASE("or_ returns the new Option", "[const-ref]") {
  const int other = 4;
  Option<const int &> a = Option<const int &>().or_(Some(other));
  REQUIRE(a.is_some());
  REQUIRE(!a.is_none());
  REQUIRE(&a.unwrap_unchecked() == &other);
}

TEST_CASE("or_else computes the new Option", "[const-ref]") {
  const int other = 4;
  Option<const int &> a = Option<const int &>().or_else([&]() { return Some(other); });
  REQUIRE(a.is_some());
  REQUIRE(!a.is_none());
  REQUIRE(&a.unwrap_unchecked() == &other);
}

TEST_CASE("or_else returns the same Option", "[const-ref]") {
  const int value = 5;
  const int other = 4;
  Option<const int &> a = Some(value).or_else([&]() { return Some(other); });
  REQUIRE(a.is_some());
  REQUIRE(!a.is_none());
  REQUIRE(&a.unwrap_unchecked() == &value);
}

TEST_CASE("xor_ returns the same Option", "[const-ref]") {
  const int value = 5;
  Option<const int &> a = Some(value).xor_(None());
  REQUIRE(a.is_some());
  REQUIRE(!a.is_none());
  REQUIRE(&a.unwrap_unchecked() == &value);
}

TEST_CASE("xor_ returns the new Option", "[const-ref]") {
  const int other = 4;
  Option<const int &> a = Option<const int &>().xor_(Some(other));
  REQUIRE(a.is_some());
  REQUIRE(!a.is_none());
  REQUIRE(&a.unwrap_unchecked() == &other);
}

TEST_CASE("xor_ returns no Option", "[const-ref]") {
  SECTION("both Options are full") {
    const int value = 5;
    const int other = 4;
    Option<const int &> a = Some(value).xor_(Some(other));
    REQUIRE(a.is_none());
    REQUIRE(!a.is_some());
  }

  SECTION("both Options are empty") {
    Option<const int &> a = Option<const int &>().xor_(None());
    REQUIRE(a.is_none());
    REQUIRE(!a.is_some());
  }
}

TEST_CASE("insert returns the new value", "[const-ref]") {
  SECTION("initial Option is empty") {
    const int value = 5;
    Option<const int &> a;
    const int &val = a.insert(value);
    REQUIRE(&val == &value);
    REQUIRE(a.is_some());
    REQUIRE(!a.is_none());
  }

  SECTION("initial Option is full") {
    const int initial = 4;
    const int newVal = 5;
    Option<const int &> a = Some(initial);
    const int &val = a.insert(newVal);
    REQUIRE(&val == &newVal);
    REQUIRE(a.is_some());
    REQUIRE(!a.is_none());
  }
}

TEST_CASE("get_or_insert returns existing value", "[const-ref]") {
  const int initial = 5;
  const int newVal = 4;
  Option<const int &> a = Some(initial);
  const int &val = a.get_or_insert(newVal);
  REQUIRE(&val == &initial);
  REQUIRE(a.is_some());
  REQUIRE(!a.is_none());
}

TEST_CASE("get_or_insert returns new value", "[const-ref]") {
  const int newVal = 4;
  Option<const int &> a;
  const int &val = a.get_or_insert(newVal);
  REQUIRE(&val == &newVal);
  REQUIRE(a.is_some());
  REQUIRE(!a.is_none());
}

TEST_CASE("get_or_insert_with returns existing value", "[const-ref]") {
  const int initial = 5;
  const int newVal = 4;
  Option<const int &> a = Some(initial);
  const int &val = a.get_or_insert_with([&]() -> const int& { return newVal; });
  REQUIRE(&val == &initial);
  REQUIRE(a.is_some());
  REQUIRE(!a.is_none());
}

TEST_CASE("get_or_insert_with returns computed value", "[const-ref]") {
  const int newVal = 4;
  Option<const int &> a = None();
  const int &val = a.get_or_insert_with([&]() -> const int &{ return newVal; });
  REQUIRE(&val == &newVal);
  REQUIRE(a.is_some());
  REQUIRE(!a.is_none());
}

TEST_CASE("take moves current Option", "[const-ref]") {
  SECTION("Option is full") {
      const int value = 5;
    Option<const int &> a = Some(value);
    Option<const int &> b = a.take();
    REQUIRE(a.is_none());
    REQUIRE(!a.is_some());
    REQUIRE(b.is_some());
    REQUIRE(!b.is_none());
    REQUIRE(&b.unwrap_unchecked() == &value);
  }

  SECTION("Option is empty") {
    Option<const int &> a = None();
    Option<const int &> b = a.take();
    REQUIRE(a.is_none());
    REQUIRE(!a.is_some());
    REQUIRE(b.is_none());
    REQUIRE(!b.is_some());
  }
}

TEST_CASE("take_if moves current Option", "[const-ref]") {
  SECTION("Option is full and predicate is true") {
      const int value = 5;
    Option<const int &> a = Some(value);
    Option<const int &> b = a.take_if([](const int &val) { return true; });
    REQUIRE(a.is_none());
    REQUIRE(!a.is_some());
    REQUIRE(b.is_some());
    REQUIRE(!b.is_none());
    REQUIRE(&b.unwrap_unchecked() == &value);
  }

  SECTION("Option is full and predicate is false") {
      const int value = 5;
    Option<const int &> a = Some(value);
    Option<const int &> b = a.take_if([](const int &val) { return false; });
    REQUIRE(a.is_some());
    REQUIRE(!a.is_none());
    REQUIRE(b.is_none());
    REQUIRE(!b.is_some());
    REQUIRE(&a.unwrap_unchecked() == &value);
  }

  SECTION("Option is empty and predicate is false") {
    Option<const int &> a = None();
    Option<const int &> b = a.take_if([](const int &val) { return false; });
    REQUIRE(a.is_none());
    REQUIRE(!a.is_some());
    REQUIRE(b.is_none());
    REQUIRE(!b.is_some());
  }

  SECTION("Option is empty and predicate is true") {
    Option<const int &> a = None();
    Option<const int &> b = a.take_if([](const int &val) { return true; });
    REQUIRE(a.is_none());
    REQUIRE(!a.is_some());
    REQUIRE(b.is_none());
    REQUIRE(!b.is_some());
  }
}

TEST_CASE("replace returns old option", "[const-ref]") {
  SECTION("Option is full") {
      const int value = 5;
      const int newValue = 4;
    Option<const int &> a = Some(value);
    Option<const int &> b = a.replace(newValue);
    REQUIRE(a.is_some());
    REQUIRE(!a.is_none());
    REQUIRE(&a.unwrap_unchecked() == &newValue);
    REQUIRE(b.is_some());
    REQUIRE(!b.is_none());
    REQUIRE(&b.unwrap_unchecked() == &value);
  }

  SECTION("Option is empty") {
      const int newValue = 4;
    Option<const int &> a = None();
    Option<const int &> b = a.replace(newValue);
    REQUIRE(a.is_some());
    REQUIRE(!a.is_none());
    REQUIRE(&a.unwrap_unchecked() == &newValue);
    REQUIRE(b.is_none());
    REQUIRE(!b.is_some());
  }
}
