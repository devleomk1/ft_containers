/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jisokang <jisokang@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/21 19:25:17 by jisokang          #+#    #+#             */
/*   Updated: 2022/10/04 15:25:35 by jisokang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//vector<bool> 전문화를 수행할 필요가 없습니다. ->

#ifndef VECTOR_HPP
# define VECTOR_HPP

# include <memory>
# include <stdexcept>
//# include <algorithm>
//# include <vector>
# include "iterator_traits.hpp"
/**
 * @brief
 *
 */
namespace ft
{
	//template <class T, class Allocator = std::allocator<T> >
	template <class T, class Alloc = std::allocator<T> >			//왜 Allocator가 아니라 Alloc으로 하는 걸까?
	class vector
	{
	public:
		//types:
		typedef typename	Allocator::reference		reference;
		typedef typename	Allocator::const_reference	const_reference;
	//ISO 14882_1998 23.1 implementation-defined ==============================
		typedef pointer									iterator;
		typedef const_pointer							const_iterator;
		typedef size_t									size_type;
		typedef ptrdiff_t								difference_type;
	//define end ==============================================================
		typedef T										value_type;
		typedef Allocator								allocator_type;
		//typedef typename	Alloc::template rebind<T>::other	allocator_type; 왜 rebind 하는 걸까?
		typedef typename	Allocator::pointer			pointer;
		typedef typename	Allocator::constr_pointer	const_pointer;
		typedef ft::reverse_iterator<iterator>			reverse_iterator;
		typedef ft::reverse_iterator<const_iterator>	conset_reverse_iterator;

		//23.2.4.1 construct/copy/destroy
		//explicit: 생성자 앞에 explicit 키워드를 붙여주면 변환 생성자의 무작위 호출을 막고 명확성을 높여준다.
		//explicit	vector(const Allocator& = Allocator())
		explicit	vector(const allocator_type& a = allocator_type())
			: ft_alloc(a), ft_start(0), ft_finish(0), ft_end_storage(0)
			{};
		//왜 allocator로 alloc을 따로 하는거지?
		//explicit	vector(size_type n, const T& value = T(), const Allocator& = Allocator())
		explicit	vector(size_type n, const T& value = T(), const allocator_type& a = Allocator())
			: _alloc(a), _start(0), _finish(0), _end_storage(0)
			{
				_start = _alloc.allocate( n );
				_end_storage = _start + n;
				_finish = _start;
				while (n--)
				{
					_alloc.construct( _finish, value);
					_finish++;
				}
			};

		template <class InputIterator>
			vector(InputIterator first, InputIterator last, const Allocator& = Allocator());
		vector(const vector<T, Allocator>& x);
		~vector();
		vector<T, Allocator>& operator=(const vector<T, Allocator>& x);
		template <class InputIterator>
			void assign(InputIterator first, InputIterator last);
		void assign(size_type n, const T& u);
		allocator_type get_allocator() const;

		//iterators:
		iterator				begin();
		const_iterator			begin() const;
		iterator				end();
		const_iterator			end() const;
		reverse_iterator		rbegin();
		const_reverse_iterator	rbegin() const;
		reverse_iterator		rend();
		const_reverse_iterator	rend() const;

		//capacity:
		size_type				size() const;
		size_type				max_size() const;
		void					resize(size_type sz, T c = T());
		size_type				capacity() const;
		bool					empty() const;
		void					reverse(size_type n);

		//element access
		reference				operator[](size_type n);
		const_reference			operator[](size_type n) const;
		reference				at(size_type n);
		const_reference			at(size_type n) const;
		reference				front();
		const_reference			front() const;
		reference				back();
		const_reference			back() const;

		//modifiers:
		void					push_back(const	T& x);
		void					pop_back();
		iterator				insert(iterator position, const T& x);
		void					insert(iterator position, size_type n, const T& x);
		template <class InputIterator>
			void				insert(iterator position, InputIterator first, InputIterator last);
		iterator				erase(iterator position);
		iterator				erase(iterator first, iterator last);
		void					swap(vector<T, Allocator>&);
		void					clear();
	protected:
		allocator_type			_alloc;
		T*						_start;
		T*						_finish;
		T*						_end_storage;
	};

}

#endif
