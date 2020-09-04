#include "comp6771/euclidean_vector.hpp"

#include <catch2/catch.hpp>
#include <fmt/format.h>
#include <fmt/ostream.h>

//-----------------------------------test constructors-------------------------------------------
TEST_CASE("default constructor: should create [0]") {
	auto const a1 = comp6771::euclidean_vector();
	CHECK(a1.dimensions() == 1);
	CHECK(a1.at(0) == 0);
}
TEST_CASE("size constructor: should create an all-0 vector with desired dimension") {
	SECTION("regular case") {
		auto const a1 = comp6771::euclidean_vector(3);
		REQUIRE(a1.dimensions() == 3);
		CHECK(a1[0] == 0);
		CHECK(a1[1] == 0);
		CHECK(a1[2] == 0);
	}
	SECTION("0 dimension") {
		auto const a1 = comp6771::euclidean_vector(0);
		REQUIRE(a1.dimensions() == 0);
	}
}
TEST_CASE("fill constructor: should create a vector whose magnitudes in every dimenison equals to "
          "specified value") {
	SECTION("regular case") {
		auto const a1 = comp6771::euclidean_vector(2, 9.8);
		REQUIRE(a1.dimensions() == 2);
		CHECK(a1[0] == 9.8);
		CHECK(a1[1] == 9.8);
	}
	SECTION("0 dimension") {
		auto const a1 = comp6771::euclidean_vector(0, 5);
		REQUIRE(a1.dimensions() == 0);
	}
}
TEST_CASE("range constructor: should copy the values in the std::vector<double>{}") {
	SECTION("regular case") {
		auto const v1 = std::vector<double>{1, 2, 4.5, 7.6};
		auto const a1 = comp6771::euclidean_vector(v1.begin(), v1.end());
		REQUIRE(a1.dimensions() == 4);
		CHECK(a1[0] == v1[0]);
		CHECK(a1[1] == v1[1]);
		CHECK(a1[2] == v1[2]);
		CHECK(a1[3] == v1[3]);
	}
	SECTION("empty double") {
		auto const v1 = std::vector<double>{};
		auto const a1 = comp6771::euclidean_vector(v1.begin(), v1.end());
		CHECK(a1.dimensions() == 0);
	}
}
TEST_CASE("initializer_list list constructor: should copy the values in the "
          "std::initializer_list<double>{}") {
	SECTION("regular case") {
		auto const a1 = comp6771::euclidean_vector{1, 2, 5.3, 7.6};
		REQUIRE(a1.dimensions() == 4);
		CHECK(a1[0] == 1);
		CHECK(a1[1] == 2);
		CHECK(a1[2] == 5.3);
		CHECK(a1[3] == 7.6);
	}
	SECTION("empty list") {
		auto const a1 = comp6771::euclidean_vector{std::initializer_list<double>{}};
		REQUIRE(a1.dimensions() == 0);
	}
}
TEST_CASE("copy constructor: shold copy the values in the original euclidean vector. The "
          "contents of the 2 objects should have different addresses") {
	auto a1 = comp6771::euclidean_vector{1, 2, 5.3, 7.6};
	auto a2 = comp6771::euclidean_vector(a1);
	REQUIRE(a2.dimensions() == 4);
	CHECK(std::addressof(a1) != std::addressof(a2));
	CHECK(std::addressof(a1[0]) != std::addressof(a2[0]));
	CHECK(a2[0] == a1[0]);
	CHECK(a2[1] == a1[1]);
	CHECK(a2[2] == a1[2]);
	CHECK(a2[3] == a1[3]);
}
TEST_CASE("move constructor: should move the contents from original vector to new vector") {
	auto a1 = comp6771::euclidean_vector{1, 2, 5.3, 7.6};
	auto const* address_of_values_a1 = std::addressof(a1[0]);

	auto a2 = comp6771::euclidean_vector(std::move(a1));
	auto const* address_of_values_a2 = std::addressof(a2[0]);

	CHECK(address_of_values_a1 == address_of_values_a2);
	REQUIRE(a2.dimensions() == 4);
	CHECK(a2[0] == 1);
	CHECK(a2[1] == 2);
	CHECK(a2[2] == 5.3);
	CHECK(a2[3] == 7.6);
}

//-----------------------------------test firend operators----------------------------------------
// I test friend operators before other parts, because I want to use "==" and "<<" in other tests
TEST_CASE("friends '==' and '!=' should be complement") {
	SECTION("self comparason: a1 == a1") {
		auto const a1 = comp6771::euclidean_vector{1, 2, 3, 4};
		CHECK(a1 == a1);
		CHECK(not(a1 != a1));
	}
	SECTION("two vectors are equal: a1 == a2") {
		auto const a1 = comp6771::euclidean_vector{1, 2, 3, 4};
		auto const a2 = comp6771::euclidean_vector{1, 2, 3, 4};
		REQUIRE(std::addressof(a1) != std::addressof(a2));
		CHECK(a1 == a2);
		CHECK(not(a1 != a2));
	}
	SECTION("some elements are different: a1[3] != a2[3] ==> a1 != a2") {
		auto const a1 = comp6771::euclidean_vector{1, 2, 3, 4};
		auto const a2 = comp6771::euclidean_vector{1, 2, 3, 5};
		CHECK(a1 != a2);
		CHECK(not(a1 == a2));
	}
	SECTION("dimension mismatch: a1.dimensions() != a2.dimensions() ==> a1 != a2") {
		auto const a1 = comp6771::euclidean_vector{1, 2, 3, 4};
		auto const a2 = comp6771::euclidean_vector{1, 2, 3};
		CHECK(a1 != a2);
		CHECK(not(a1 == a2));
	}
	SECTION("two vectors are equal after changing element values") {
		auto const a1 = comp6771::euclidean_vector{1, 2, 3, 4};
		auto a2 = comp6771::euclidean_vector{1, 2, 3, 5};
		CHECK(a1 != a2);
		CHECK(not(a1 == a2));
		a2[3] = a1[3];
		CHECK(a1 == a2);
		CHECK(not(a1 != a2));
	}
}
TEST_CASE("friend: output stream <<: [all magnitudes]") {
	SECTION("regular test") {
		auto const a1 = comp6771::euclidean_vector{1, 2, 3, 3.2};
		CHECK(fmt::format("{}", a1) == "[1 2 3 3.2]");

		auto a2 = comp6771::euclidean_vector{-9};
		CHECK(fmt::format("{}", a2) == "[-9]");
	}
	SECTION("0-dimensional vector should output []") {
		auto const a1 = comp6771::euclidean_vector(0);
		CHECK(fmt::format("{}", a1) == "[]");
	}
	SECTION("output stream should change with the change of magnitudes") {
		auto a1 = comp6771::euclidean_vector{1, 3, -9};
		CHECK(fmt::format("{}", a1) == "[1 3 -9]");
		a1[0] = 0;
		a1[1] = -99;
		CHECK(fmt::format("{}", a1) == "[0 -99 -9]");
	}
}
TEST_CASE("friend: + -") {
	SECTION("regular addition") {
		auto const a1 = comp6771::euclidean_vector{1, 2, 3};
		auto const a2 = comp6771::euclidean_vector{0, 0, 0};
		auto a3 = a1 + a2;
		CHECK(a1 == a3);
		a3 = a1 + comp6771::euclidean_vector{2, 2, 2};
		CHECK(a3 == comp6771::euclidean_vector{3, 4, 5});
	}
	SECTION("regular subtraction") {
		auto const a1 = comp6771::euclidean_vector{1, 2, 3.5};
		auto const a2 = comp6771::euclidean_vector{0, 0, 0};
		auto a3 = a1 - a2;
		CHECK(a1 == a3);
		a3 = a1 - comp6771::euclidean_vector{2, 2, 2};
		CHECK(a3 == comp6771::euclidean_vector{-1, 0, 1.5});
	}
	SECTION("self addition") {
		auto const a1 = comp6771::euclidean_vector{1.2, 2.2, 3.2};
		auto a2 = a1 + a1;
		CHECK(a2 == comp6771::euclidean_vector{2.4, 4.4, 6.4});
	}
	SECTION("self subtraction") {
		auto const a1 = comp6771::euclidean_vector{1.2, 2.2, 3.2};
		// Turning off clang check for redundant expression to check self-subtraction
		// NOLINTNEXTLINE(misc-redundant-expression)
		auto a2 = a1 - a1;
		CHECK(a2 == comp6771::euclidean_vector{0, 0, 0});
	}
	SECTION("addition exception: dimension mismatch") {
		auto a1 = comp6771::euclidean_vector{0, 0, 0};
		auto a2 = comp6771::euclidean_vector{0, 0};
		auto const message = std::string("Dimensions of LHS(X) and RHS(Y) do not match");
		REQUIRE_THROWS(a1 + a2);
		CHECK_THROWS_MATCHES(a1 + a2,
		                     comp6771::euclidean_vector_error,
		                     Catch::Matchers::Message(message));
	}
	SECTION("subtraction exception: dimension mismatch") {
		auto a1 = comp6771::euclidean_vector{0, 0, 0};
		auto a2 = comp6771::euclidean_vector{0, 0};
		auto const message = std::string("Dimensions of LHS(X) and RHS(Y) do not match");
		REQUIRE_THROWS(a1 - a2);
		CHECK_THROWS_MATCHES(a1 - a2,
		                     comp6771::euclidean_vector_error,
		                     Catch::Matchers::Message(message));
	}
}

TEST_CASE("friend: * /") {
	SECTION("regular multiplication") {
		auto const a1 = comp6771::euclidean_vector{1, 2.2, 3};
		auto a2 = a1 * 0;
		CHECK(a2 == comp6771::euclidean_vector{0, 0, 0});
		a2 = a1 * 3;
		CHECK(a2 == comp6771::euclidean_vector{3, 6.6, 9});
	}
	SECTION("regular division") {
		auto const a1 = comp6771::euclidean_vector{9, 6, 3};
		auto a2 = a1 / 0.5;
		CHECK(a2 == comp6771::euclidean_vector{18, 12, 6});
		a2 = a1 / 3;
		CHECK(a2 == comp6771::euclidean_vector{3, 2, 1});
	}
	SECTION("0 division exception") {
		auto a1 = comp6771::euclidean_vector{0, 0, 0};
		auto a2 = comp6771::euclidean_vector{0, 0};
		auto const message = std::string("Dimensions of LHS(X) and RHS(Y) do not match");
		REQUIRE_THROWS(a1 - a2);
		CHECK_THROWS_MATCHES(a1 - a2,
		                     comp6771::euclidean_vector_error,
		                     Catch::Matchers::Message(message));
	}
}
//----------------------------------------operators----------------------------------------------
TEST_CASE("copy assignment: should copy the vector in a new memory address") {
	auto a1 = comp6771::euclidean_vector{1, 3, -2, -9.5};
	auto a2 = comp6771::euclidean_vector();
	auto const* address_of_contents_a1 = std::addressof(a1[0]);
	a2 = a1;
	auto const* address_of_contents_a2 = std::addressof(a2[0]);
	CHECK(address_of_contents_a1 != address_of_contents_a2); // different memory address
	CHECK(a1 == a2); // same content
}
TEST_CASE("move assignment: move the contents of the original vector to the new vector") {
	auto a1 = comp6771::euclidean_vector{1, 3, -2, -9.5};
	auto a2 = comp6771::euclidean_vector();
	auto const* address_of_contents_a1 = std::addressof(a1[0]);
	a2 = std::move(a1);
	auto const* address_of_contents_a2 = std::addressof(a2[0]);
	CHECK(address_of_contents_a1 == address_of_contents_a2); // different memory address
	CHECK(a2 == comp6771::euclidean_vector{1, 3, -2, -9.5});
}
TEST_CASE("subscripts") {
	SECTION("non-const [] should surpport reading and modification") {
		auto a1 = comp6771::euclidean_vector{1, 3, -2, -9.5};
		CHECK(a1[0] == 1);
		CHECK(a1[1] == 3);
		CHECK(a1[2] == -2);
		CHECK(a1[3] == -9.5);

		a1[0] = 9999;
		CHECK(a1[0] == 9999);
		CHECK(a1 == comp6771::euclidean_vector{9999, 3, -2, -9.5});
	}
	SECTION("const [] should surpport reading") {
		auto const a1 = comp6771::euclidean_vector{1, 3, -2, -9.5};
		CHECK(a1[0] == 1);
		CHECK(a1[1] == 3);
		CHECK(a1[2] == -2);
		CHECK(a1[3] == -9.5);
	}
}

TEST_CASE("unary +: return a copy of current object") {
	SECTION("regular case") {
		auto a1 = comp6771::euclidean_vector{1, 2, 5.3, 7.6};
		auto a2 = comp6771::euclidean_vector();
		a2 = +a1;
		REQUIRE(a2.dimensions() == 4);
		CHECK(a2 == a1);

		auto const* address_of_contents_a1 = std::addressof(a1[0]);
		auto const* address_of_contents_a2 = std::addressof(a2[0]);
		CHECK(address_of_contents_a1 != address_of_contents_a2);
	}
	SECTION("assign + to self should change the address") {
		auto a1 = comp6771::euclidean_vector{1, 2, 5.3, 7.6};
		auto const* address_of_contents_orig = std::addressof(a1[0]);
		a1 = +a1;
		auto const* address_of_contents_new = std::addressof(a1[0]);
		CHECK(address_of_contents_orig != address_of_contents_new);
		CHECK(a1 == comp6771::euclidean_vector{1, 2, 5.3, 7.6});
	}
}
TEST_CASE("negation -:") {
	auto const a1 = comp6771::euclidean_vector{1, 2, 5.3, 7.6};
	auto const a2 = -a1;
	auto const desired_result = comp6771::euclidean_vector{-1, -2, -5.3, -7.6};
	CHECK(a2 == desired_result);
}
TEST_CASE("Compound Addition += ") {
	SECTION("regular case") {
		auto a1 = comp6771::euclidean_vector{1, 2, 3, 4};
		auto const a2 = comp6771::euclidean_vector{1, 1, 1, 0};
		a1 += a2;
		CHECK(a1 == comp6771::euclidean_vector{2, 3, 4, 4});
	}
	SECTION("self compound addition") {
		auto a1 = comp6771::euclidean_vector{1, 2, 3, 4};
		a1 += a1;
		CHECK(a1 == comp6771::euclidean_vector{2, 4, 6, 8});
	}
	SECTION("exception: dimension mismatch") {
		auto a1 = comp6771::euclidean_vector{0, 1};
		auto a2 = comp6771::euclidean_vector{0, 1, 2};
		auto const message = std::string("Dimensions of LHS(X) and RHS(Y) do not match");
		REQUIRE_THROWS(a1 += a2);
		CHECK_THROWS_MATCHES(a1 += a2,
		                     comp6771::euclidean_vector_error,
		                     Catch::Matchers::Message(message));
	}
}
TEST_CASE("Compound subtraction -= ") {
	SECTION("regular case") {
		auto a1 = comp6771::euclidean_vector{1, 2, 3, 4};
		auto const a2 = comp6771::euclidean_vector{1, 1, 1, 0};
		a1 -= a2;
		CHECK(a1 == comp6771::euclidean_vector{0, 1, 2, 4});
	}
	// SECTION("self compound subtraction") {
	// 	auto a1 = comp6771::euclidean_vector{1, 2, 3, 4};
	// 	// NOLINTNEXTLINE(clang-diagnostic-self-assign-overloaded)
	// 	a1 -= a1;
	// 	CHECK(a1 == comp6771::euclidean_vector{0, 0, 0, 0});
	// }
	SECTION("exception: dimension mismatch") {
		auto a1 = comp6771::euclidean_vector{0, 1};
		auto a2 = comp6771::euclidean_vector{0, 1, 2};
		auto const message = std::string("Dimensions of LHS(X) and RHS(Y) do not match");
		REQUIRE_THROWS(a1 -= a2);
		CHECK_THROWS_MATCHES(a1 -= a2,
		                     comp6771::euclidean_vector_error,
		                     Catch::Matchers::Message(message));
	}
}
TEST_CASE("Compound Multiplication *=") {
	auto a1 = comp6771::euclidean_vector{1.1, -2.2, 3.3};
	a1 *= 3;
	CHECK(a1 == comp6771::euclidean_vector{3.3, -6.6, 9.9});
}
TEST_CASE("Compound Multiplication /=") {
	SECTION("regular case") {
		auto a1 = comp6771::euclidean_vector{3.3, -6.6, 9.9};
		a1 /= -3;
		CHECK(a1 == comp6771::euclidean_vector{-1.1, 2.2, -3.3});
	}
	SECTION("exception: 0 division") {
		auto a1 = comp6771::euclidean_vector{3.3, -6.6, 9.9};
		auto const message = std::string("Invalid vector division by 0");
		REQUIRE_THROWS(a1 /= 0);
		CHECK_THROWS_MATCHES(a1 /= 0,
		                     comp6771::euclidean_vector_error,
		                     Catch::Matchers::Message(message));
	}
}

TEST_CASE("vector type conversions: should return a std::vector<double> containing the "
          "magnitudes") {
	auto a1 = comp6771::euclidean_vector{3.3, -6.6, 9.9};
	auto std_vec = static_cast<std::vector<double>>(a1);

	REQUIRE(std_vec.size() == 3);
	CHECK(std_vec[0] == a1[0]);
	CHECK(std_vec[1] == a1[1]);
	CHECK(std_vec[2] == a1[2]);
}

TEST_CASE("list type conversions: should return a std::list<double> containing the "
          "magnitudes") {
	auto a1 = comp6771::euclidean_vector{3.3, -6.6, 9.9};
	auto std_list = static_cast<std::list<double>>(a1);

	REQUIRE(std_list.size() == 3);
	auto iter_list = std_list.begin();
	CHECK(*iter_list == a1[0]);
	++iter_list;
	CHECK(*iter_list == a1[1]);
	++iter_list;
	CHECK(*iter_list == a1[2]);
	++iter_list;
	CHECK(iter_list == std_list.end());
}

//-----------------------------------------member functiond---------------------------------------
TEST_CASE("const at(): return the value of magnitude in the desired dimension, support reading "
          "only") {
	SECTION("regular case") {
		auto const a1 = comp6771::euclidean_vector{1, 2.5, 3};
		CHECK(a1.at(0) == 1);
		CHECK(a1.at(1) == 2.5);
		CHECK(a1.at(2) == 3);
	}
	SECTION("exception: invalid index / index out of range") {
		auto const a1 = comp6771::euclidean_vector{1, 2.5, 3};
		auto const message = std::string("Index X is not valid for this euclidean_vector object");
		CHECK_THROWS_MATCHES(a1.at(-1),
		                     comp6771::euclidean_vector_error,
		                     Catch::Matchers::Message(message));
		CHECK_THROWS_MATCHES(a1.at(5),
		                     comp6771::euclidean_vector_error,
		                     Catch::Matchers::Message(message));
	}
}

TEST_CASE("non-const at(): return the value of magnitude in the desired dimension, support reading "
          "and modification") {
	SECTION("regular case") {
		auto a1 = comp6771::euclidean_vector{1, 2.5, 3};
		CHECK(a1.at(0) == 1);
		CHECK(a1.at(1) == 2.5);
		CHECK(a1.at(2) == 3);

		a1.at(2) = -99;
		CHECK(a1.at(2) == -99);
		CHECK(a1 == comp6771::euclidean_vector{1, 2.5, -99});
	}
	SECTION("exception: invalid index / index out of range") {
		auto const a1 = comp6771::euclidean_vector{1, 2.5, 3};
		auto const message = std::string("Index X is not valid for this euclidean_vector object");
		CHECK_THROWS_MATCHES(a1.at(-1),
		                     comp6771::euclidean_vector_error,
		                     Catch::Matchers::Message(message));
		CHECK_THROWS_MATCHES(a1.at(5),
		                     comp6771::euclidean_vector_error,
		                     Catch::Matchers::Message(message));
	}
}

TEST_CASE("dimensions()") {
	auto a1 = comp6771::euclidean_vector{1, 2, 3};
	CHECK(a1.dimensions() == 3);

	auto const a2 = comp6771::euclidean_vector(0);
	CHECK(a2.dimensions() == 0);

	auto const a3 = comp6771::euclidean_vector(999, 99);
	CHECK(a3.dimensions() == 999);
}

//------------------------------------Utility functions------------------------------------------
TEST_CASE("euclidean_norm(vec): return euclidean norm value") {
	SECTION("regular case") {
		auto a1 = comp6771::euclidean_vector{0, 0, 0, 5, 0};
		CHECK(comp6771::euclidean_norm(a1) == 5);

		auto const a2 = comp6771::euclidean_vector{3, 4, 3, 4, -3, -4, 3, 4};
		CHECK(comp6771::euclidean_norm(a2) == 10);
	}
	SECTION("0 norm") {
		auto const a1 = comp6771::euclidean_vector{0, 0};
		CHECK(comp6771::euclidean_norm(a1) == 0);
	}
	SECTION("exception: 0 dimension vector") {
		auto a1 = comp6771::euclidean_vector(0);
		auto const message = std::string("euclidean_vector with no dimensions does not have a norm");
		CHECK_THROWS_MATCHES(comp6771::euclidean_norm(a1),
		                     comp6771::euclidean_vector_error,
		                     Catch::Matchers::Message(message));
	}
}

TEST_CASE("unit(vec): return unit vector") {
	SECTION("regualr case") {
		auto const a1 = comp6771::euclidean_vector{0, 5, 0};
		CHECK(comp6771::unit(a1) == comp6771::euclidean_vector{0, 1, 0});

		auto const a2 = comp6771::euclidean_vector{-3, -4};
		CHECK(comp6771::unit(a2) == comp6771::euclidean_vector{-0.6, -0.8});
	}
	SECTION("exception: 0 dimension vector") {
		auto a1 = comp6771::euclidean_vector(0);
		auto const message = std::string("euclidean_vector with no dimensions does not have a unit "
		                                 "vector");
		CHECK_THROWS_MATCHES(comp6771::unit(a1),
		                     comp6771::euclidean_vector_error,
		                     Catch::Matchers::Message(message));
	}
	SECTION("exception: 0 normed vector doesn't have unit vector") {
		auto a1 = comp6771::euclidean_vector{0, 0, 0, 0, 0};
		auto const message = std::string("euclidean_vector with zero euclidean normal does not have "
		                                 "a unit vector");
		CHECK_THROWS_MATCHES(comp6771::unit(a1),
		                     comp6771::euclidean_vector_error,
		                     Catch::Matchers::Message(message));
	}
}

TEST_CASE("dot(vec1, vec2): return dot product of 2 euclidean vectors") {
	SECTION("regular case") {
		auto const a1 = comp6771::euclidean_vector{1, 2, 3};
		auto const a2 = comp6771::euclidean_vector{3, -2, 1};
		auto const result = 1.0 * 3.0 + (-2.0) * 2.0 + 3.0 * 1.0;
		CHECK(comp6771::dot(a1, a2) == result);
	}
	SECTION("dot product with a zero vector should be 0") {
		auto a1 = comp6771::euclidean_vector{2, 43, 1, -654, 2};
		auto a2 = comp6771::euclidean_vector(5);
		CHECK(comp6771::dot(a1, a2) == 0);
	}
	SECTION("exception: dimension mismatch") {
		auto const a1 = comp6771::euclidean_vector{2, 43, 1, -654, 2};
		auto const a2 = comp6771::euclidean_vector{1, 3, 2};
		auto const message = std::string("Dimensions of LHS(X) and RHS(Y) do not match");
		CHECK_THROWS_MATCHES(comp6771::dot(a1, a2),
		                     comp6771::euclidean_vector_error,
		                     Catch::Matchers::Message(message));
	}
}