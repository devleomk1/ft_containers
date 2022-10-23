/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   iterator.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jisokang <jisokang@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/23 20:35:21 by jisokang          #+#    #+#             */
/*   Updated: 2022/10/23 18:03:46 by jisokang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ITERATOR_HPP
# define ITERATOR_HPP

# include <cstddef>				// ptrdiff_t (https://en.cppreference.com/w/cpp/types/ptrdiff_t)
//# include "iterator_traits.hpp"

namespace ft
{
	template<class Iterator> struct iterator_traits
	{
		typedef typename Iterator::difference_type		difference_type;
		typedef typename Iterator::value_type			value_type;
		typedef typename Iterator::pointer				pointer;
		typedef typename Iterator::reference			reference;
		typedef typename Iterator::iterator_category	iterator_category;
	};

	template<class T> struct iterator_traits<T*>
	{
		typedef ptrdiff_t								difference_type;
		typedef T										value_type;
		typedef T*										pointer;
		typedef T&										reference;
		typedef std::random_access_iterator_tag			iterator_category;
	};

	template<class T> struct iterator_traits<const T*>
	{
		typedef ptrdiff_t								difference_type;
		typedef T										value_type;
		typedef const T									pointer;
		typedef const T&								reference;
		typedef std::random_access_iterator_tag			iterator_category;
	};

	template<class Category, class T, class Distance = ptrdiff_t, class Pointer = T*, class Reference = T&>
	struct iterator
	{
		typedef T			value_type;
		typedef Distance	difference_type;
		typedef Pointer		pointer;
		typedef Reference	reference;
		typedef Category	iterator_category;
	};

template <class Iterator>
	class reverse_iterator : public
		iterator<typename iterator_traits<Iterator>::iterator_category,
				 typename iterator_traits<Iterator>::value_type,
				 typename iterator_traits<Iterator>::difference_type,
				 typename iterator_traits<Iterator>::pointer,
				 typename iterator_traits<Iterator>::reference> {
	public:
		typedef Iterator											iterator_type;
		typedef typename iterator_traits<Iterator>::difference_type	difference_type;
		typedef typename iterator_traits<Iterator>::reference		reference;
		typedef typename iterator_traits<Iterator>::pointer			pointer;

	protected:
		Iterator current;

		reverse_iterator()
		{};
		explicit reverse_iterator(Iterator x)
		: current(x) {};
		template <class U> reverse_iterator(const reverse_iterator<U>& u)
		: current(u.base()) {};

		iterator_type	base() const{
			return(current);
		};
		reference	operator*() const{
			Iterator tmp = current;
			return *--tmp;
		};
		pointer		operator->() const{
			return &(operator*());
		};

		reverse_iterator&	operator++(){
			--current;
			return (*this);
		};
		reverse_iterator	operator++(int){
			reverse_iterator tmp = *this;
			--current;
			return (tmp);
		};
		reverse_iterator&	operator--(){
			++current;
			return (*this);
		};
		reverse_iterator	operator--(int){
			reverse_iterator tmp = *this;
			++current;
			return (tmp);
		};

		reverse_iterator	operator+ (typename reverse_iterator<Iterator>::difference_type n) const{
			return (reverse_iterator(current - n));
		};
		reverse_iterator&	operator+=(typename reverse_iterator<Iterator>::difference_type n){
			current -= n;
			return (*this);
		};
		reverse_iterator	operator- (typename reverse_iterator<Iterator>::difference_type n) const{
			return (reverse_iterator(current + n));
		};
		reverse_iterator&	operator-=(typename reverse_iterator<Iterator>::difference_type n){
			current += n;
			return (*this);
		};
		reference operator[](typename reverse_iterator<Iterator>::difference_type n) const{
			return (current[-n-1]);	//왜에?
		};
	};

	// 🚨
	// Q1. template <class Iterator> Header에는 이렇게 써있는데 왜 Class 2개 받는거임???
	// Q2. current로 받아도 되는거 아닌가? 왜 base()로 받지?
	//template <class Iterator>
	//	bool operator==(
	//		const reverse_iterator<Iterator>& x,
	//		const reverse_iterator<Iterator>& y)
	//		{
	//			return (x.current == y.current);
	//		};
	//template <class Iterator>
	//	bool operator<(
	//		const reverse_iterator<Iterator>& x,
	//		const reverse_iterator<Iterator>& y)
	//		{
	//			return (x.current > y.current);
	//		};
	//template <class Iterator>
	//	bool operator!=(
	//		const reverse_iterator<Iterator>& x,
	//		const reverse_iterator<Iterator>& y)
	//		{
	//			return (x.current != y.current);
	//		};
	//template <class Iterator>
	//	bool operator>(
	//		const reverse_iterator<Iterator>& x,
	//		const reverse_iterator<Iterator>& y)
	//		{
	//			return (x.current < y.current);
	//		};
	//template <class Iterator>
	//	bool operator>=(
	//		const reverse_iterator<Iterator>& x,
	//		const reverse_iterator<Iterator>& y)
	//		{
	//			return (x.current <= y.current);
	//		};
	//template <class Iterator>
	//	bool operator<=(
	//		const reverse_iterator<Iterator>& x,
	//		const reverse_iterator<Iterator>& y)
	//		{
	//			return (x.current >= y.current);
	//		};
	//template <class Iterator>
	//	typename reverse_iterator<Iterator>::difference_type operator-(	//왜 difference_type으로 가져올까?
	//		const reverse_iterator<Iterator>& x,
	//		const reverse_iterator<Iterator>& y)
	//		{
	//			return (y.current - x.current);
	//		};
	//template <class Iterator>
	//	typename reverse_iterator<Iterator>::difference_type operator+(	//왜 -n 으로 인자를 받을까 y가 아니고?
	//		typename reverse_iterator<Iterator>::difference_type n,
	//		const reverse_iterator<Iterator>& x)
	//		{
	//			return (x.current - n);
	//		};

	template <class IteratorL, class IteratorR>
	bool operator==(
			const reverse_iterator<IteratorL>& x,
			const reverse_iterator<IteratorR>& y) {
		return x.base() == y.base();
	}

	template <class IteratorL, class IteratorR>
	bool operator<(
			const reverse_iterator<IteratorL>& x,
			const reverse_iterator<IteratorR>& y) {
		return x.base() > y.base();
	}

	template <class IteratorL, class IteratorR>
	bool operator!=(
			const reverse_iterator<IteratorL>& x,
			const reverse_iterator<IteratorR>& y) {
		return x.base() != y.base();
	}

	template <class IteratorL, class IteratorR>
	bool operator>(
			const reverse_iterator<IteratorL>& x,
			const reverse_iterator<IteratorR>& y) {
		return x.base() < y.base();
	}

	template <class IteratorL, class IteratorR>
	bool operator>=(
			const reverse_iterator<IteratorL>& x,
			const reverse_iterator<IteratorR>& y) {
		return x.base() <= y.base();
	}

	template <class IteratorL, class IteratorR>
	bool operator<=(
			const reverse_iterator<IteratorL>& x,
			const reverse_iterator<IteratorR>& y) {
		return x.base() >= y.base();
	}

	template <class IteratorL, class IteratorR>
	typename reverse_iterator<IteratorL>::difference_type operator-(
			const reverse_iterator<IteratorL>& x,
			const reverse_iterator<IteratorR>& y) {
		return y.base() - x.base();
	}

	template <class Iterator>
	reverse_iterator<Iterator> operator+(
			typename reverse_iterator<Iterator>::difference_type n,
			const reverse_iterator<Iterator>& x) {
		return reverse_iterator<Iterator> (x.base() - n);
	}

	////24.3.4 iterator operations:
	//// advance와 distance는 필요 없는가??
	//template <class InputIterator, class Distance>
	//	void advance(InputIterator& i, Distance n){
	//			//????
	//			//없어서 안했음
	//	};
	//template <class InputIterator>
	//	typename iterator_traits<InputIterator>::difference_type
	//	distance(InputIterator first, InputIterator last){
	//			//????
	//			//이건 쓰긴함
	//			// 두 이터레이터 사이의 길이 연산하는거
	//	};


}

#endif
