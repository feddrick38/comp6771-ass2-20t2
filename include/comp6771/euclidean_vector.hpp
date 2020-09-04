#ifndef COMP6771_EUCLIDEAN_VECTOR_HPP
#define COMP6771_EUCLIDEAN_VECTOR_HPP

#include <compare>
#include <functional>
#include <list>
#include <memory>
#include <ostream>
#include <range/v3/algorithm.hpp>
#include <range/v3/iterator.hpp>
#include <span>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

namespace comp6771 {
	class euclidean_vector_error : public std::runtime_error {
	public:
		explicit euclidean_vector_error(std::string const& what) noexcept
		: std::runtime_error(what) {}
	};

	class euclidean_vector {
	public:
		//------------------------threshold for firend == -------------------------
		static double constexpr epsilon = 0.0000001;

		//----------------------------constructors---------------------------------
		euclidean_vector() noexcept;
		explicit euclidean_vector(int) noexcept;
		euclidean_vector(int, double) noexcept;
		euclidean_vector(std::vector<double>::const_iterator,
		                 std::vector<double>::const_iterator) noexcept;
		euclidean_vector(std::initializer_list<double>) noexcept;
		euclidean_vector(euclidean_vector const&) noexcept;
		euclidean_vector(euclidean_vector&&) noexcept;

		//---------------------------destructor------------------------------------
		~euclidean_vector() = default;

		//---------------------------operators-------------------------------------
		auto operator=(euclidean_vector const&) noexcept -> euclidean_vector&;
		auto operator=(euclidean_vector&&) noexcept -> euclidean_vector&;
		auto operator[](int) noexcept -> double&;
		auto operator[](int) const noexcept -> double;
		auto operator+() const noexcept -> euclidean_vector;
		auto operator-() const noexcept -> euclidean_vector;
		auto operator+=(euclidean_vector const&) -> euclidean_vector&;
		auto operator-=(euclidean_vector const&) -> euclidean_vector&;
		auto operator*=(double) noexcept -> euclidean_vector&;
		auto operator/=(double) -> euclidean_vector&;
		explicit operator std::vector<double>() const noexcept;
		explicit operator std::list<double>() const noexcept;

		//-----------------------member functions----------------------------------
		[[nodiscard]] auto at(int) const -> double;
		[[nodiscard]] auto at(int) -> double&;
		[[nodiscard]] auto dimensions() const noexcept -> int;

		//--------------------------friends----------------------------------------
		friend auto operator==(euclidean_vector const&, euclidean_vector const&) noexcept -> bool;
		friend auto operator!=(euclidean_vector const&, euclidean_vector const&) noexcept -> bool;
		friend auto operator+(euclidean_vector const& lhs, euclidean_vector const& rhs)
		   -> euclidean_vector;
		friend auto operator-(euclidean_vector const& lhs, euclidean_vector const& rhs)
		   -> euclidean_vector;
		friend auto operator*(euclidean_vector const&, double) noexcept -> euclidean_vector;
		friend auto operator/(euclidean_vector const&, double) -> euclidean_vector;
		friend auto operator<<(std::ostream&, euclidean_vector const&) noexcept -> std::ostream&;

		//----------------------Utility functions----------------------------------
		friend auto euclidean_norm(euclidean_vector const& v) -> double;
		friend auto unit(euclidean_vector const& v) -> euclidean_vector;
		friend auto dot(euclidean_vector const& x, euclidean_vector const& y) -> double;

	private:
		//-----------------------artributes----------------------------------------
		int dimensions_;
		// NOLINTNEXTLINE(modernize-avoid-c-arrays)
		std::unique_ptr<double[]> magnitudes_;
	};
	auto euclidean_norm(euclidean_vector const& v) -> double;
	auto unit(euclidean_vector const& v) -> euclidean_vector;
	auto dot(euclidean_vector const& x, euclidean_vector const& y) -> double;
} // namespace comp6771
#endif // COMP6771_EUCLIDEAN_VECTOR_HPP
