#include <catch2/catch_test_macros.hpp>

#include "../Option.hpp"

using namespace option;

TEST_CASE("Initialize Option with None structure", "[value]") {
    Option<int> opt = None();
    REQUIRE(opt.is_none());
    REQUIRE(!opt.is_some());
}

TEST_CASE("Option default constructor", "[value]") {
    Option<int> opt;
    REQUIRE(opt.is_none());
    REQUIRE(!opt.is_some());
}

TEST_CASE("Initialize Option with Some function", "[value]") {
    Option<int> opt = Some(5);
    REQUIRE(opt.is_some());
    REQUIRE(!opt.is_none());
    REQUIRE(opt.unwrap_unchecked() == 5);
}

TEST_CASE("is_some_and returns true on a full Option with a true predicate", "[value]") {
    int actual = 0;
    REQUIRE(Some(5).is_some_and([&](int &&val){ actual = val; return true;}));
    REQUIRE(actual == 5);
}

TEST_CASE("is_some_and returns false on a full Option with a false predicate", "[value]") {
    REQUIRE(!Some(5).is_some_and([](int &&val){ return false;}));
}

TEST_CASE("is_some_and returns false on an empty Option with a true predicate", "[value]") {
    REQUIRE(!Option<int>().is_some_and([](int &&val){ return true;}));
}

TEST_CASE("is_some_and returns false on an empty Option with a false predicate", "[value]") {
    REQUIRE(!Option<int>().is_some_and([](int &&val){ return false;}));
}

TEST_CASE("as_ref returns an Option containing a reference to the same value", "[value]") {
    Option<int> a = Some(5);
    Option<const int &> b = a.as_ref();
    REQUIRE(b.is_some());
    REQUIRE(!b.is_none());
    REQUIRE(b.unwrap_unchecked() == 5);
}

TEST_CASE("as_ref returns an emption Option from an empty Option", "[value]") {
    Option<int> a = None();
    Option<const int &> b = a.as_ref();
    REQUIRE(b.is_none());
    REQUIRE(!b.is_some());
}

TEST_CASE("as_mut returns an Option containing a reference to the same value", "[value]") {
    Option<int> a = Some(5);
    Option<int &> b = a.as_mut();
    REQUIRE(b.is_some());
    REQUIRE(!b.is_none());
    REQUIRE(b.unwrap_unchecked() == 5);
}

TEST_CASE("as_mut returns an emption Option from an empty Option", "[value]") {
    Option<int> a = None();
    Option<int &> b = a.as_mut();
    REQUIRE(b.is_none());
    REQUIRE(!b.is_some());
}

TEST_CASE("expect returns the value on a full Option", "[value]") {
    REQUIRE(Some(5).expect("Something is wrong with Option::expect") == 5);
}

TEST_CASE("unwrap returns the value on a full Option", "[value]") {
    REQUIRE(Some(5).unwrap() == 5);
}

TEST_CASE("unwrap_or returns the value on a full Option", "[value]") {
    REQUIRE(Some(5).unwrap_or(6) == 5);
}

TEST_CASE("unwrap_or returns the other value on an empty Option", "[value]") {
    REQUIRE(Option<int>().unwrap_or(6) == 6);
}

TEST_CASE("unwrap_or_else returns other value on a full Option", "[value]") {
    REQUIRE(Some(5).unwrap_or_else([](){ return 6; }) == 5);
}

TEST_CASE("unwrap_or_else returns the other value on an empty Option", "[value]") {
    REQUIRE(Option<int>().unwrap_or_else([](){ return 6; }) == 6);
}

TEST_CASE("unwrap_or_default returns a default value on an empty Option", "[value]") {
    REQUIRE(Option<int>().unwrap_or_default() == 0);
}

TEST_CASE("unwrap_or_default returns the value on a full Option", "[value]") {
    REQUIRE(Option<int>(5).unwrap_or_default() == 5);
}

TEST_CASE("unwrap_unchecked returns the value on a full Option", "[value]") {
    REQUIRE(Option<int>(5).unwrap_unchecked() == 5);
}

TEST_CASE("map returns a new Option", "[value]") {
    Option<double> a = Some(5).map([](int &&val){ return 2.0; });
    REQUIRE(a.is_some());
    REQUIRE(!a.is_none());
    REQUIRE(a.unwrap_unchecked() == 2.0);
}

TEST_CASE("map returns another empty Option", "[value]") {
    Option<double> a = Option<int>().map([](int &&val){ return 2.0; });
    REQUIRE(a.is_none());
    REQUIRE(!a.is_some());
}

TEST_CASE("map receives the correct value", "[value]") {
    int actual = 0;
    Some(5).map([&](int &&val){ actual = val; return 2.0; });
    REQUIRE(actual == 5);
}

TEST_CASE("inspect returns a full Option", "[value]") {
    int actual = 0;
    Option<int> a = Some(5).inspect([&](const int &val){ actual = val;});
    REQUIRE(a.is_some());
    REQUIRE(!a.is_none());
    REQUIRE(a.unwrap_unchecked() == 5);
    REQUIRE(actual == 5);
}

TEST_CASE("inspect returns an empty option", "[value]") {
    int actual = 0;
    Option<int> a = Option<int>().inspect([&](const int &val){ actual = val;});
    REQUIRE(a.is_none());
    REQUIRE(!a.is_some());
    REQUIRE(actual == 0);
}

TEST_CASE("map_or computes a new value", "[value]") {
    int actual = 0;
    Option<double> a = Some(5).map_or(1.0, [&](int &&val){ actual = val; return 2.0; });
    REQUIRE(a.is_some());
    REQUIRE(!a.is_none());
    REQUIRE(a.unwrap_unchecked() == 2.0);
    REQUIRE(actual == 5);
}

TEST_CASE("map_or returns the default value", "[value]") {
    int actual = 0;
    Option<double> a = Option<int>().map_or(1.0, [&](int &&val){ actual = val; return 2.0; });
    REQUIRE(a.is_some());
    REQUIRE(!a.is_none());
    REQUIRE(a.unwrap_unchecked() == 1.0);
    REQUIRE(actual == 0);
}

TEST_CASE("map_or_else computes the default value", "[value]") {
    int actual = 0;
    Option<double> a = Option<int>().map_or_else([](){ return 1.0; }, [&](int &&val){ actual = val; return 2.0; });
    REQUIRE(a.is_some());
    REQUIRE(!a.is_none());
    REQUIRE(a.unwrap_unchecked() == 1.0);
    REQUIRE(actual == 0);
}

TEST_CASE("map_or_else computes a new value", "[value]") {
    int actual = 0;
    Option<double> a = Some(5).map_or_else([](){ return 1.0; }, [&](int &&val){ actual = val; return 2.0; });
    REQUIRE(a.is_some());
    REQUIRE(!a.is_none());
    REQUIRE(a.unwrap_unchecked() == 2.0);
    REQUIRE(actual == 5);
}

TEST_CASE("and_ returns the new Option", "[value]") {
    Option<double> a = Some(5).and_(Some(2.0));
    REQUIRE(a.is_some());
    REQUIRE(!a.is_none());
    REQUIRE(a.unwrap_unchecked() == 2.0);
}

TEST_CASE("and_ returns an empty Option", "[value]") {
    Option<double> a = Option<int>().and_(Some(2.0));
    REQUIRE(a.is_none());
    REQUIRE(!a.is_some());
}

TEST_CASE("and_then returns an empty Option", "[value]") {
    Option<double> a = Option<int>().and_then([](int &&val){ return Some(2.0); });
    REQUIRE(a.is_none());
    REQUIRE(!a.is_some());
}

TEST_CASE("and_then returns a new full Option", "[value]") {
    int actual = 0;
    Option<double> a = Some(5).and_then([&](int &&val){ actual = val; return Some(2.0); });
    REQUIRE(a.is_some());
    REQUIRE(!a.is_none());
    REQUIRE(a.unwrap_unchecked() == 2.0);
    REQUIRE(actual == 5);
}

TEST_CASE("and_then returns a new empty Option", "[value]") {
    int actual = 0;
    Option<double> a = Some(5).and_then([&](int &&val){ actual = val; return Option<double>(); });
    REQUIRE(a.is_none());
    REQUIRE(!a.is_some());
    REQUIRE(actual == 5);
}

TEST_CASE("filter returns the same Option when given a true predicate", "[value]") {
    int actual = 0;
    Option<int> a = Some(5).filter([&](const int &val){ actual = val; return true;});
    REQUIRE(a.is_some());
    REQUIRE(!a.is_none());
    REQUIRE(a.unwrap_unchecked() == 5);
    REQUIRE(actual == 5);
}

TEST_CASE("filter returns an empty Option when given a false predicate", "[value]") {
    int actual = 0;
    Option<int> a = Some(5).filter([&](const int &val){ actual = val; return false;});
    REQUIRE(a.is_none());
    REQUIRE(!a.is_some());
    REQUIRE(actual == 5);
}

TEST_CASE("filter returns an empty Option when empty", "[value]") {
    int actual = 0;
    Option<int> a = Option<int>().filter([&](const int &val){ actual = val; return true;});
    REQUIRE(a.is_none());
    REQUIRE(!a.is_some());
    REQUIRE(actual == 0);
}

TEST_CASE("or_ returns the same Option", "[value]") {
    Option<int> a = Some(5).or_(Some(4));
    REQUIRE(a.is_some());
    REQUIRE(!a.is_none());
    REQUIRE(a.unwrap_unchecked() == 5);
}

TEST_CASE("or_ returns the new Option", "[value]") {
    Option<int> a = Option<int>().or_(Some(4));
    REQUIRE(a.is_some());
    REQUIRE(!a.is_none());
    REQUIRE(a.unwrap_unchecked() == 4);
}

TEST_CASE("or_else computes the new Option", "[value]") {
    Option<int> a = Option<int>().or_else([](){ return Some(4); });
    REQUIRE(a.is_some());
    REQUIRE(!a.is_none());
    REQUIRE(a.unwrap_unchecked() == 4);
}

TEST_CASE("or_else returns the same Option", "[value]") {
    Option<int> a = Some(5).or_else([](){ return Some(4); });
    REQUIRE(a.is_some());
    REQUIRE(!a.is_none());
    REQUIRE(a.unwrap_unchecked() == 5);
}

TEST_CASE("xor_ returns the same Option", "[value]") {
    Option<int> a = Some(5).xor_(None());
    REQUIRE(a.is_some());
    REQUIRE(!a.is_none());
    REQUIRE(a.unwrap_unchecked() == 5);
}

TEST_CASE("xor_ returns the new Option", "[value]") {
    Option<int> a = Option<int>().xor_(Some(4));
    REQUIRE(a.is_some());
    REQUIRE(!a.is_none());
    REQUIRE(a.unwrap_unchecked() == 4);
}

TEST_CASE("xor_ returns no Option", "[value]") {
    SECTION("both Options are full") {
        Option<int> a = Some(5).xor_(Some(4));
        REQUIRE(a.is_none());
        REQUIRE(!a.is_some());
    }

    SECTION("both Options are empty") {
        Option<int> a = Option<int>().xor_(Option<int>());
        REQUIRE(a.is_none());
        REQUIRE(!a.is_some());
    }
}

TEST_CASE("insert returns the new value", "[value]") {
    SECTION("initial Option is empty") {
        Option<int> a;
        int &val = a.insert(5);
        REQUIRE(val == 5);
        REQUIRE(a.is_some());
        REQUIRE(!a.is_none());
    }

    SECTION("initial Option is full") {
        Option<int> a = Some(4);
        int &val = a.insert(5);
        REQUIRE(val == 5);
        REQUIRE(a.is_some());
        REQUIRE(!a.is_none());
    }
}

TEST_CASE("get_or_insert returns existing value", "[value]") {
    Option<int> a = Some(5);
    int &val = a.get_or_insert(4);
    REQUIRE(val == 5);
    REQUIRE(a.is_some());
    REQUIRE(!a.is_none());
}

TEST_CASE("get_or_insert returns new value", "[value]") {
    Option<int> a;
    int &val = a.get_or_insert(4);
    REQUIRE(val == 4);
    REQUIRE(a.is_some());
    REQUIRE(!a.is_none());
}

TEST_CASE("get_or_insert_default returns existing value", "[value]") {
    Option<int> a = Some(5);
    int &val = a.get_or_insert_default();
    REQUIRE(val == 5);
    REQUIRE(a.is_some());
    REQUIRE(!a.is_none());
}

TEST_CASE("get_or_insert_default returns default value", "[value]") {
    Option<int> a = None();
    int &val = a.get_or_insert_default();
    REQUIRE(val == 0);
    REQUIRE(a.is_some());
    REQUIRE(!a.is_none());
}

TEST_CASE("get_or_insert_with returns existing value", "[value]") {
    Option<int> a = Some(5);
    int &val = a.get_or_insert_with([](){ return 4; });
    REQUIRE(val == 5);
    REQUIRE(a.is_some());
    REQUIRE(!a.is_none());
}

TEST_CASE("get_or_insert_with returns computed value", "[value]") {
    Option<int> a = None();
    int &val = a.get_or_insert_with([](){ return 4; });
    REQUIRE(val == 4);
    REQUIRE(a.is_some());
    REQUIRE(!a.is_none());
}

TEST_CASE("take moves current Option", "[value]") {
    SECTION("Option is full") {
        Option<int> a = Some(5);
        Option<int> b = a.take();
        REQUIRE(a.is_none());
        REQUIRE(!a.is_some());
        REQUIRE(b.is_some());
        REQUIRE(!b.is_none());
        REQUIRE(b.unwrap_unchecked() == 5);
    }

    SECTION("Option is empty") {
        Option<int> a = None();
        Option<int> b = a.take();
        REQUIRE(a.is_none());
        REQUIRE(!a.is_some());
        REQUIRE(b.is_none());
        REQUIRE(!b.is_some());
    }
}

TEST_CASE("take_if moves current Option", "[value]") {
    SECTION("Option is full and predicate is true") {
        Option<int> a = Some(5);
        Option<int> b = a.take_if([](int &val){ return true; });
        REQUIRE(a.is_none());
        REQUIRE(!a.is_some());
        REQUIRE(b.is_some());
        REQUIRE(!b.is_none());
        REQUIRE(b.unwrap_unchecked() == 5);
    }

    SECTION("Option is full and predicate is false") {
        Option<int> a = Some(5);
        Option<int> b = a.take_if([](int &val){ return false; });
        REQUIRE(a.is_some());
        REQUIRE(!a.is_none());
        REQUIRE(b.is_none());
        REQUIRE(!b.is_some());
        REQUIRE(a.unwrap_unchecked() == 5);
    }

    SECTION("Option is empty and predicate is false") {
        Option<int> a = None();
        Option<int> b = a.take_if([](int &val){ return false; });
        REQUIRE(a.is_none());
        REQUIRE(!a.is_some());
        REQUIRE(b.is_none());
        REQUIRE(!b.is_some());
    }

    SECTION("Option is empty and predicate is true") {
        Option<int> a = None();
        Option<int> b = a.take_if([](int &val){ return true; });
        REQUIRE(a.is_none());
        REQUIRE(!a.is_some());
        REQUIRE(b.is_none());
        REQUIRE(!b.is_some());
    }
}

TEST_CASE("replace returns old option", "[value]") {
    SECTION("Option is full") {
        Option<int> a = Some(5);
        Option<int> b = a.replace(4);
        REQUIRE(a.is_some());
        REQUIRE(!a.is_none());
        REQUIRE(a.unwrap_unchecked() == 4);
        REQUIRE(b.is_some());
        REQUIRE(!b.is_none());
        REQUIRE(b.unwrap_unchecked() == 5);
    }

    SECTION("Option is empty") {
        Option<int> a = None();
        Option<int> b = a.replace(4);
        REQUIRE(a.is_some());
        REQUIRE(!a.is_none());
        REQUIRE(a.unwrap_unchecked() == 4);
        REQUIRE(b.is_none());
        REQUIRE(!b.is_some());
    }
}
