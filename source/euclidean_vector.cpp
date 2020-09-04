// Copyright (c) Christopher Di Bella.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
#include "comp6771/euclidean_vector.hpp"
#include <gsl/gsl-lite.hpp>
#include <range/v3/numeric/accumulate.hpp>
#include <range/v3/range.hpp>
#include <range/v3/view.hpp>
// why can compile here but not in master?
namespace comp6771 {
	//------------------------------constructors---------------------------------------------------
	// defualt constructor
	euclidean_vector::euclidean_vector() noexcept
	: euclidean_vector(1) {}

	// size constructor
	euclidean_vector::euclidean_vector(int dimension) noexcept
	: euclidean_vector(dimension, 0) {}

	// fill constructor
	euclidean_vector::euclidean_vector(int dimensions, double value) noexcept
	: dimensions_{dimensions} {
		// size_t may have different bits than int in some machines
		// so I cast int to unsigned int (not size_t)
		auto const dim_size = gsl_lite::narrow_cast<unsigned int>(dimensions_);
		// NOLINTNEXTLINE(modernize-avoid-c-arrays)
		magnitudes_ = std::make_unique<double[]>(dim_size);
		auto usable_data = std::span<double>(magnitudes_.get(), dim_size);
		ranges::fill(usable_data, value); // wow, span is so convinient
	}

	// range constructor
	euclidean_vector::euclidean_vector(std::vector<double>::const_iterator begin_iter,
	                                   std::vector<double>::const_iterator end_iter) noexcept
	: dimensions_{static_cast<int>(ranges::distance(begin_iter, end_iter))} {
		auto const dim_size = gsl_lite::narrow_cast<unsigned int>(dimensions_);
		// NOLINTNEXTLINE(modernize-avoid-c-arrays)
		magnitudes_ = std::make_unique<double[]>(dim_size);
		auto usable_data = std::span<double>(magnitudes_.get(), dim_size);
		ranges::copy(begin_iter, end_iter, usable_data.begin());
	}

	// initializer list constructor
	euclidean_vector::euclidean_vector(std::initializer_list<double> list) noexcept
	: dimensions_{static_cast<int>(ranges::distance(list))} {
		auto const dim_size = gsl_lite::narrow_cast<unsigned int>(dimensions_);
		// NOLINTNEXTLINE(modernize-avoid-c-arrays)
		magnitudes_ = std::make_unique<double[]>(dim_size);
		auto usable_data = std::span<double>(magnitudes_.get(), dim_size);
		ranges::copy(list.begin(), list.end(), usable_data.begin());
	}

	// copy constructor
	euclidean_vector::euclidean_vector(euclidean_vector const& orig) noexcept
	: dimensions_{orig.dimensions_} {
		auto const dim_size = gsl_lite::narrow_cast<unsigned int>(dimensions_);
		// NOLINTNEXTLINE(modernize-avoid-c-arrays)
		magnitudes_ = std::make_unique<double[]>(dim_size);
		auto usable_data = std::span<double>(magnitudes_.get(), dim_size);
		auto const orig_data = std::span<double>(orig.magnitudes_.get(), dim_size);
		ranges::copy(orig_data.begin(), orig_data.end(), usable_data.begin());
	}

	// move constructor
	euclidean_vector::euclidean_vector(euclidean_vector&& orig) noexcept
	: dimensions_{orig.dimensions_} {
		magnitudes_ = std::move(orig.magnitudes_);
		orig.dimensions_ = 0;
	}

	//--------------------------------operations---------------------------------------------------
	auto euclidean_vector::operator=(euclidean_vector const& oth) noexcept -> euclidean_vector& {
		// if I instead use std::addressof(*this) == std::addressof(orig), a error message regarding
		// self-assignment will occur
		if (this == &oth) {
			return *this;
		}
		auto const new_dim_size = gsl_lite::narrow_cast<unsigned int>(oth.dimensions_);
		if (dimensions_ != oth.dimensions_) {
			dimensions_ = oth.dimensions_;
			// NOLINTNEXTLINE(modernize-avoid-c-arrays)
			magnitudes_ = std::make_unique<double[]>(new_dim_size);
		}
		auto usable_data = std::span<double>(magnitudes_.get(), new_dim_size);
		auto const orig_data = std::span<double>(oth.magnitudes_.get(), new_dim_size);
		ranges::copy(orig_data.begin(), orig_data.end(), usable_data.begin());
		return *this;
	}

	auto euclidean_vector::operator=(euclidean_vector&& oth) noexcept -> euclidean_vector& {
		if (this == &oth) { // same object
			return *this;
		}
		dimensions_ = oth.dimensions_;
		magnitudes_ = std::move(oth.magnitudes_);
		oth.dimensions_ = 0;
		return *this;
	}
	auto euclidean_vector::operator[](int i) noexcept -> double& {
		assert(i >= 0 and i < dimensions_);
		return magnitudes_[gsl_lite::narrow_cast<unsigned int>(i)];
	}
	auto euclidean_vector::operator[](int i) const noexcept -> double {
		assert(i >= 0 and i < dimensions_);
		return magnitudes_[gsl_lite::narrow_cast<unsigned int>(i)];
	}

	auto euclidean_vector::operator+() const noexcept -> euclidean_vector {
		return *this;
	}
	auto euclidean_vector::operator-() const noexcept -> euclidean_vector {
		auto v = static_cast<std::vector<double>>(*this);
		ranges::transform(v, v.begin(), std::negate<>());
		return euclidean_vector(v.begin(), v.end());
	}

	auto euclidean_vector::operator+=(euclidean_vector const& oth) -> euclidean_vector& {
		if (dimensions_ != oth.dimensions_) {
			throw euclidean_vector_error("Dimensions of LHS(X) and RHS(Y) do not match");
		}
		auto const dim_size = gsl_lite::narrow_cast<unsigned int>(dimensions_);
		auto usable_data = std::span<double>(magnitudes_.get(), dim_size);
		auto const oth_data = std::span<double>(oth.magnitudes_.get(), dim_size);
		ranges::transform(usable_data, oth_data, usable_data.begin(), std::plus<>());
		return *this;
	}
	auto euclidean_vector::operator-=(euclidean_vector const& oth) -> euclidean_vector& {
		return (*this) += (-oth);
	}
	auto euclidean_vector::operator*=(double factor) noexcept -> euclidean_vector& {
		auto usable_data =
		   std::span<double>(magnitudes_.get(), gsl_lite::narrow_cast<unsigned int>(dimensions_));
		// usable_data = usable_data | views::transform([&factor](double& d) { return d * factor; });
		ranges::transform(usable_data, usable_data.begin(), [&factor](double& d) { return d * factor; });
		return *this;
	}
	auto euclidean_vector::operator/=(double dividend) -> euclidean_vector& {
		if (dividend == 0) {
			throw euclidean_vector_error("Invalid vector division by 0");
		}
		auto usable_data =
		   std::span<double>(magnitudes_.get(), gsl_lite::narrow_cast<unsigned int>(dimensions_));
		ranges::transform(usable_data, usable_data.begin(), [&dividend](double& d) {
			return d / dividend;
		});

		return *this;
	}
	euclidean_vector::operator std::vector<double>() const noexcept {
		return std::span<double>(magnitudes_.get(), gsl_lite::narrow_cast<unsigned int>(dimensions_))
		       | ranges::to<std::vector>;
	}
	euclidean_vector::operator std::list<double>() const noexcept {
		return std::span<double>(magnitudes_.get(), gsl_lite::narrow_cast<unsigned int>(dimensions_))
		       | ranges::to<std::list>;
	}

	//---------------------------------Member Functions--------------------------------------------
	auto euclidean_vector::at(int index) const -> double {
		if (index < 0 or index >= gsl_lite::narrow_cast<int>(dimensions_)) {
			throw euclidean_vector_error("Index X is not valid for this euclidean_vector object");
		}
		return magnitudes_[gsl_lite::narrow_cast<unsigned int>(index)];
	}
	auto euclidean_vector::at(int index) -> double& {
		if (index < 0 or index > gsl_lite::narrow_cast<int>(dimensions_)) {
			throw euclidean_vector_error("Index X is not valid for this euclidean_vector object");
		}
		return magnitudes_[gsl_lite::narrow_cast<unsigned int>(index)];
	}
	auto euclidean_vector::dimensions() const noexcept -> int {
		return dimensions_;
	}
	//----------------------------------friends----------------------------------------------------
	auto operator==(euclidean_vector const& lhs, euclidean_vector const& rhs) noexcept -> bool {
		if (std::addressof(lhs) == std::addressof(rhs)) { // same object
			return true;
		}
		if (lhs.dimensions() != rhs.dimensions()) {
			return false;
		}
		auto const dim_size = gsl_lite::narrow_cast<unsigned int>(lhs.dimensions_);
		auto const data_lhs = std::span<double>(lhs.magnitudes_.get(), dim_size);
		auto const data_rhs = std::span<double>(rhs.magnitudes_.get(), dim_size);
		auto const epsilon = euclidean_vector::epsilon;
		return ranges::equal(data_lhs, data_rhs, [&epsilon](double const& l, double const& r) {
			return std::abs(l - r) <= epsilon;
		});
	}
	auto operator!=(euclidean_vector const& lhs, euclidean_vector const& rhs) noexcept -> bool {
		return not(lhs == rhs);
	}
	auto operator+(euclidean_vector const& lhs, euclidean_vector const& rhs) -> euclidean_vector {
		auto result = euclidean_vector(lhs);
		result += rhs; // dimension not match exception can raise inside "+="
		return result;
	}
	auto operator-(euclidean_vector const& lhs, euclidean_vector const& rhs) -> euclidean_vector {
		return lhs + (-rhs);
	}
	auto operator*(euclidean_vector const& vec, double factor) noexcept -> euclidean_vector {
		auto result = euclidean_vector(vec);
		result *= factor;
		return result;
	}
	auto operator/(euclidean_vector const& vec, double divident) -> euclidean_vector {
		auto result = euclidean_vector(vec);
		result /= divident;
		return result;
	}
	auto operator<<(std::ostream& os, euclidean_vector const& vec) noexcept -> std::ostream& {
		if (vec.dimensions() == 0) {
			return os << "[]";
		}
		os << '[';
		auto usable_data = std::span<double>(vec.magnitudes_.get(),
		                                     gsl_lite::narrow_cast<unsigned int>(vec.dimensions_));
		// ranges::copy() doesn't work here
		std::copy(usable_data.begin(), usable_data.end() - 1, std::ostream_iterator<double>(os, " "));
		return os << usable_data.back() << ']';
	}

	//-------------------------------Utility functions---------------------------------------------
	auto euclidean_norm(euclidean_vector const& v) -> double {
		if (v.dimensions() == 0) {
			throw euclidean_vector_error("euclidean_vector with no dimensions does not have a norm");
		}
		return std::sqrt(dot(v, v));
	}

	auto unit(euclidean_vector const& v) -> euclidean_vector {
		if (v.dimensions() == 0) {
			throw euclidean_vector_error("euclidean_vector with no dimensions does not have a unit "
			                             "vector");
		}
		auto const norm = euclidean_norm(v);
		if (norm == 0) {
			throw euclidean_vector_error("euclidean_vector with zero euclidean normal does not "
			                             "have a unit vector");
		}
		return v / norm;
	}

	auto dot(euclidean_vector const& x, euclidean_vector const& y) -> double {
		if (x.dimensions() != y.dimensions()) {
			throw euclidean_vector_error("Dimensions of LHS(X) and RHS(Y) do not match");
		}
		auto const dim_size = gsl_lite::narrow_cast<unsigned int>(x.dimensions_);
		auto const x_data = std::span<double>(x.magnitudes_.get(), dim_size);
		auto const y_data = std::span<double>(y.magnitudes_.get(), dim_size);

		auto element_wise_mult = std::vector<double>(dim_size);
		ranges::transform(x_data, y_data, element_wise_mult.begin(), std::multiplies<>());

		return ranges::accumulate(element_wise_mult, 0);
	}
} // namespace comp6771
