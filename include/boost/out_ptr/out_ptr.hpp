//  Copyright ⓒ 2018-2019 ThePhD.
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://www.boost.org/libs/out_ptr/ for documentation.

#pragma once

#ifndef BOOST_OUT_PTR_HPP
#define BOOST_OUT_PTR_HPP

#include <boost/out_ptr/simple_out_ptr.hpp>
#include <boost/out_ptr/clever_out_ptr.hpp>

#include <type_traits>
#include <tuple>

namespace boost {

	namespace detail {
#if defined(BOOST_OUT_PTR_NO_CLEVERNESS) && BOOST_OUT_PTR_NO_CLEVERNESS != 0
		template <typename Smart, typename Pointer, typename... Args>
		using core_out_ptr_t = simple_out_ptr_t<Smart, Pointer, Args...>;
#else
		template <typename Smart, typename Pointer, typename... Args>
		using core_out_ptr_t = clever_out_ptr_t<Smart, Pointer, Args...>;
#endif // BOOST_OUT_PTR_NO_CLEVERNESS
	} // namespace detail

	template <typename Smart, typename Pointer, typename... Args>
	class out_ptr_t : public detail::core_out_ptr_t<Smart, Pointer, Args...> {
	private:
		using base_t = detail::core_out_ptr_t<Smart, Pointer, Args...>;

	public:
		using base_t::base_t;
	};

	template <typename Pointer, typename Smart, typename... Args>
	out_ptr_t<Smart, Pointer, Args...> out_ptr(Smart& s, Args&&... args) noexcept {
		using P = out_ptr_t<Smart, Pointer, Args...>;
		return P(s, std::forward<Args>(args)...);
	}

	template <typename Smart, typename... Args>
	out_ptr_t<Smart, pointer_of_t<Smart>, Args...> out_ptr(Smart& s, Args&&... args) noexcept {
		using Pointer = pointer_of_t<Smart>;
		using P	  = out_ptr_t<Smart, Pointer, Args...>;
		return P(s, std::forward<Args>(args)...);
	}

} // namespace boost

#endif // BOOST_OUT_PTR_HPP
