/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jisokang <jisokang@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/21 19:25:17 by jisokang          #+#    #+#             */
/*   Updated: 2022/11/12 19:28:46 by jisokang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// 💡 vector<bool> 전문화를 수행할 필요가 없습니다.

#ifndef VECTOR_HPP
# define VECTOR_HPP

# include <memory>
//# include <algorithm>
# include <stdexcept>
# include "type_traits.hpp"
# include "iterator.hpp"
# include "algorithm.hpp"

namespace ft
{
	//template <class T, class Allocator = std::allocator<T> -> 내가 만든 얼록케이터라는것을 확실하게 하기 위해서
	template <class T, class Alloc = std::allocator<T> >
	class vector
	{
	public:
		//types:
		typedef T												value_type;
		/**
		 * @brief rebind
		 *
		 * 결과적으로, list<T>는 allocator::rebind<_Node>::other를 참조함으로써,
		 * T 객체의 할당자를 통해 _Node의 할당자를 찾을 수 있게 된다.
		 * 그리고, 표준 컨테이너는 반드시 이 rebind 중첩 구조체 템플릿이 있을 것으로 가정하고 작성되어 있다.
		 * 이것이 바로 STL allocator가 반드시 rebind 중첩 구조체 템플릿을 가져야 하는 이유이다.
		 *
		 */
		typedef typename	Alloc::template rebind<T>::other	allocator_type;

		//typedef Allocator								allocator_type;
		typedef typename	allocator_type::reference			reference;
		typedef typename	allocator_type::const_reference		const_reference;
		typedef typename	allocator_type::pointer				pointer;
		typedef typename	allocator_type::const_pointer		const_pointer;
	// ISO 14882_1998 - 23.1 implementation-defined ============================
		typedef pointer											iterator;
		typedef const_pointer									const_iterator;
		typedef size_t											size_type;
		typedef ptrdiff_t										difference_type;
	// define end ==============================================================
		typedef ft::reverse_iterator<iterator>					reverse_iterator;
		typedef ft::reverse_iterator<const_iterator>			const_reverse_iterator;

		//23.2.4.1 construct/copy/destroy
		//explicit: 생성자 앞에 explicit 키워드를 붙여주면 변환 생성자의 무작위 호출을 막고 명확성을 높여준다.
		explicit	vector(const allocator_type& alloc = allocator_type())
			: _alloc(alloc), _start(0), _finish(0), _end_storage(0)
			{};
		explicit	vector(size_type n, const T& value = value_type(),
			const allocator_type& alloc = allocator_type())
			: _alloc(alloc)
			{
				_start = _alloc.allocate( n );
				_end_storage = _start + n;
				iterator i = _start;
				while (n--)
				{
					_alloc.construct(i, value);
					i++;
				}
				_finish = i;
			};

		template <class InputIterator>
			vector(InputIterator first, InputIterator last, const allocator_type& alloc = allocator_type(),
					typename ft::enable_if<!ft::is_integral<InputIterator>::value, InputIterator>::type* = 0)
					: _alloc(alloc)
			{
				size_type n = std::distance(first, last);
				_start = _alloc.allocate(n);
				_end_storage = _start + n;
				pointer i = _start;
				while (first != last)
				{
					_alloc.construct(i, *first);
					i++;
					first++;
				}
				_finish = i;
			};

		vector(const vector<T, Alloc>& x) : _alloc(x._alloc)
		{
			size_type n = x.size();
			_start = _alloc.allocate( n );
			_end_storage = _start + n;
			iterator i = _start;
			iterator j = x._start;
			while (n--)
			{
				_alloc.construct( i, *j );
				j++;
				i++;
			}
			_finish = i;
		};

		~vector()
		{
			iterator i = _start;
			while (i != _finish)
			{
				_alloc.destroy(i);
				i++;
			}
			_alloc.deallocate(_start, _end_storage - _start);
		};
		vector<T, Alloc>& operator=(const vector<T, Alloc>& x)
		{
			if (&x != this)
			{
				const	size_type x_len = x.size();
				//x가 나보다 크면
				if (x_len > capacity())
				{
					pointer	new_start = _alloc.allocate(x_len);
					pointer	i = new_start;
					pointer j = const_cast<pointer>(x.begin());		//const_cast : refernce의 const를 잠깐 제거해주는 기능
					while (j != x.end())
					{
						_alloc.construct(i, *j);
						i++;
						j++;
					}
					iterator k = _start;
					while (k != _finish)
					{
						_alloc.destroy(k);
						k++;
					}
					_alloc.deallocate(_start, _end_storage - _start);
					_start = new_start;
					_end_storage = _start + x_len;
				}
				//내가 x보다 크거나 같으면
				else if (size() >= x_len)
				{
					iterator i(std::copy(x.begin(), x.end(), begin()));
					while (i != end())
					{
						_alloc.destroy(i);
						i++;
					}
				}
				else
				{
					std::copy(x.begin(), x.begin() + size(), _start);
					iterator it1 = _finish;
					iterator it2 = const_cast<pointer>(x.begin()) + size();
					while (it2 != x.end())
					{
						_alloc.construct(it1, *it2);
						it1++;
						it2++;
					}
				}
				_finish = _start + x_len;
			}
			return (*this);
		};

		template <class InputIterator>
			void assign(InputIterator first, InputIterator last,
			typename ft::enable_if<!ft::is_integral<InputIterator>::value, InputIterator>::type* = 0)
			{
				erase(begin(), end());
				insert(begin(), first, last);
			};

		void assign(size_type n, const T& u)
		{
			erase(begin(), end());
			insert(begin(), n, u);
		};
		allocator_type get_allocator() const{
			return ( allocator_type() );
		};

		//iterators:
		iterator				begin(){
			return ( iterator(_start) );
		};
		const_iterator			begin() const{
			return ( const_iterator(_start) );
		};
		iterator				end(){
			return ( iterator(_finish) );
		};
		const_iterator			end() const{
			return ( const_iterator(_finish) );
		};
		reverse_iterator		rbegin(){
			return ( reverse_iterator(end()) );
		};
		const_reverse_iterator	rbegin() const{
			return ( const_reverse_iterator(end()) );
		};
		reverse_iterator		rend(){
			return ( reverse_iterator(begin()) );
		};
		const_reverse_iterator	rend() const{
			return ( const_reverse_iterator(begin()) );
		};

		//capacity:
		size_type				size() const {
			return ( size_type(end() - begin()) );
		};
		/**
		 * @brief Returns the maximum number of elements that the vector can hold.
		 *
		 * @return size_type
		 */
		size_type				max_size() const{
			return ( _alloc.max_size() );
		};
		void					resize(size_type n, value_type v = value_type()){
			if ( n > size() ){
				insert(end(), n - size(), v);
			}
			else if (n < size()){
				erase(begin() + n, end());
			}
			else{
				/* Same, Nothing to do*/
			}
		};
		size_type				capacity() const
		{
			return ( size_type(const_iterator(_end_storage) - begin()) );
		};
		bool					empty() const
		{
			return ( begin() == end() );
		};
		/**
		 * @brief n개의 원소를 저장할 위치를 예약합니다(미리 동적할당 해놉니다)
		 *
		 * @param n
		 */
		void					reserve(size_type n)
		{
			if (capacity() < n)
			{
				if (n > max_size()){	//ex: INT_MAX
					throw std::length_error("vector");
				}
				const size_type old_size = size();
				pointer new_start(_alloc.allocate(n));
				std::copy(_start, _finish, new_start);
				iterator it = _start;
				while (it != _finish)
				{
					_alloc.destroy(it);
					it++;
				}
				_alloc.deallocate(_start, _end_storage - _start);
				_start = new_start;
				_finish = new_start + old_size;
				_end_storage = new_start + n;
			}
		};

		//element access
		reference				operator[](size_type n){
			//NO RANGE CHECK REQUIRED
			return ( *(begin() + n) );
		};
		const_reference			operator[](size_type n) const{
			//NO RANGE CHECK REQUIRED
			return ( *(begin() + n) );
		};
		reference				at(size_type n){
			_range_check(n);
			return ( (*this)[n] );
		};
		const_reference			at(size_type n) const{
			_range_check(n);
			return ( (*this)[n] );
		};
		reference				front(){
			return ( *begin() );
		};
		const_reference			front() const{
			return ( *begin() );
		};
		reference				back(){
			return ( *(end() - 1) );
		};
		const_reference			back() const{
			return ( *(end() - 1) );
		};

		//modifiers:
		void					push_back(const	T& x){
			if (_finish != _end_storage)
			{
				_alloc.construct(_finish, x);
				_finish++;
			}
			else
			{
				const size_type old_size = size();
				const size_type len = old_size != 0 ? 2 * old_size : 1;
				iterator new_start(_alloc.allocate(len));
				iterator new_finish(new_finish);
				iterator i = new_start;
				iterator j = _start;
				while (j != _finish)
				{
					_alloc.construct(i, *j);
					i++;
					j++;
				}
				new_finish = i;
				_alloc.construct(new_finish, x);
				new_finish++;
				iterator it = _start;
				while (it != _finish)
				{
					_alloc.destroy(it);
					it++;
				}
				_alloc.deallocate(_start, _end_storage - _start);
				_start = new_start;
				_finish = new_finish;
				_end_storage = new_start + len;

			}

		};

		/**
		 * @brief 마지막 원소를 only 삭제만
		 *
		 */
		void					pop_back(){
			_finish--;
			_alloc.destroy(_finish);	//들어 있는 데이터를 삭제
		};

		/**
		 * @brief 삽입
		 *
		 * @param position	: 삽입할 위치
		 * @param x			: 삽입할 친구
		 * @return iterator
		 */
		iterator				insert(iterator position, const T& x){
			size_type n	= position - begin();
			if (_finish != _end_storage)
			{
				if (position == end())	//end()에 삽입할 때
				{
					_alloc.construct(_finish, x);
					_finish++;
				}
				else //( position != end() )
				{
					_alloc.construct(_finish, *(_finish -1));
					++_finish;
					value_type	x_copy = x;
					iterator	it_i = _finish - 2;
					iterator	it_j = _finish - 1;
					while (it_i != position)
					{
						*--it_j = *--it_i;
					}
					*position = x_copy;
				}
			}
			else	// ( _finish == _end_storage )
			{
				const size_type	old_size = size();
				const size_type	len = old_size != 0 ? 2 * old_size : 1;	//pls change if
				iterator		new_start(_alloc.allocate(len));
				iterator		new_finish(new_start);
				iterator		it_i = new_start;
				iterator		it_j = _start;

				while (it_j != position)
				{
					_alloc.construct(it_i, *it_j);
					it_i++;
					it_j++;
				}
				new_finish = it_i;
				_alloc.construct(new_finish, x);
				new_finish++;
				it_i = new_finish;
				it_j = position;
				while (it_j != _finish)
				{
					_alloc.construct(it_i, *it_j);
					it_i++;
					it_j++;
				}
				new_finish = it_i;
				it_i = _start;
				while (it_i != _finish)
				{
					_alloc.destroy(it_i);
					it_i++;
				}
				_alloc.deallocate(_start, _end_storage - _start);
				_start = new_start;
				_finish = new_finish;
				_end_storage = new_start + len;
			}
			return ( begin() + n );

		};

		void					insert(iterator position, size_type n, const T& x)
		{
			if ( n == 0 )
			{
				return ;
			}
			if ( size_type(_end_storage - _finish) >= n )
			{
				const size_type	elem_after = end() - position;	//why elem_after?
				iterator		old_finish(_finish);

				if ( elem_after > n )
				{
					iterator	it_i = _finish;
					iterator	it_j = _finish - n;
					while (it_j != _finish)
					{
						_alloc.construct(it_i, *it_j);
						it_i++;
						it_j++;
					}
					_finish += n;
					it_i = old_finish - n;
					it_j = old_finish;
					while (it_i != position)
					{
						*--it_j = *--it_i;
					}
					while (n > 0)
					{
						value_type x_copy = x;
						*position = x_copy;
						n--;
						position++;
					}


				}
				else /* ( elem_after <= n ) */
				{
					iterator	it_i = _finish;
					n -= elem_after;
					while ( n > 0 )
					{
						value_type x_copy = x;
						_alloc.construct(it_i, x_copy);
						it_i++;
						n--;
					}
					iterator	it_j = position;
					while (it_j != old_finish)
					{
						_alloc.construct(it_i, *it_j);
						it_i++;
						it_j++;
					}
					_finish = it_i;
					while (position != old_finish)
					{
						value_type x_copy = x;
						*position = x_copy;
						position++;
					}
				}
			}
			else // ( size_type(_end_storage - _finish) < n )
			{
				const size_type	old_size = size();
				const size_type	len = old_size + std::max(old_size, n);
				iterator		new_start(_alloc.allocate(len));
				iterator		new_finish(new_start);
				iterator		it_i = new_start;
				iterator		it_j = _start;
				while ( it_j != position )
				{
					_alloc.construct(it_i, *it_j);
					it_i++;
					it_j++;
				}
				new_finish = it_i;
				while ( n > 0 )
				{
					_alloc.construct(it_i, x);
					it_i++;
					n--;
				}
				new_finish = it_i;
				it_j = position;
				while ( it_j != _finish )
				{
					_alloc.construct(it_i, *it_j);
					it_i++;
					it_j++;
				}
				new_finish = it_i;
				iterator		it_k = _start;
				while (it_k != _finish)
				{
					_alloc.destroy(it_k);
					it_k++;
				}
				_alloc.deallocate(_start, _end_storage - _start);
				_start = new_start;
				_finish = new_finish;
				_end_storage = new_start + len;
			}

		};

		template <class InputIterator>
			void				insert(iterator position, InputIterator first, InputIterator last,
										typename ft::enable_if<!ft::is_integral<InputIterator>::value, InputIterator>::type* = 0)
			{
				while (first != last)
				{
					position = insert(position, *first);
					position++;
					first++;
				}
			};

		/**
		 * @brief position 위치의 원소 하나를 지우고 지워진 원소의 다음 위치를 가르킨다.
		 *
		 * @param position  : 지울 원소 위치
		 * @return iterator : 지워진 원소의 다음 위치
		 */
		iterator				erase(iterator position){
			std::copy(position + 1, end(), position);
			_finish--;
			_alloc.destroy(_finish);	//소멸자 호출
			return ( position );
		};

		/**
		 * @brief first와 last사이의 모든 원소를 지운다.
		 *
		 * @param first : 시작 원소
		 * @param last  : 끝 원소
		 * @return iterator
		 */
		iterator				erase(iterator first, iterator last){
			iterator it(std::copy(last, end(), first));
			while (it != end())
			{
				_alloc.destroy(it);
				it++;
			}
			_finish = _finish - (last - first);
			return ( first );
		};

		/**
		 * @brief swap!
		 *
		 */
		void					swap(vector<T, Alloc>& x){
			std::swap(_start, x._start);
			std::swap(_finish, x._finish);
			std::swap(_end_storage, x._end_storage);
		};

		/**
		 * @brief 모든 원소 삭 to the 제
		 *
		 */
		void					clear(){
			erase(begin(), end());
		};


	protected:
		allocator_type			_alloc;
		T*						_start;
		T*						_finish;
		T*						_end_storage;

	private:
		void	_range_check(size_type n) const{
			if (n >= size())
			{
				throw std::out_of_range("vector");
			}
		}
	};

	/**
	 * @brief Non-member function overloads
	 * - relational operators
	 * 		operation	|	equivalent operation
	 *		a!=b		|	!(a==b)
	 *		a>b			|	b<a
	 *		a<=b		|	!(b<a)
	 *		a>=b		|	!(a<b)
	 * - swap
	 */
	template <class T, class Alloc>
		bool operator== (const vector<T,Alloc>& x, const vector<T,Alloc>& y){
			return ( x.size() == y.size() && ft::equal(x.begin(), x.end(), y.begin()) );
		};
	template <class T, class Alloc>
		bool operator!= (const vector<T,Alloc>& x, const vector<T,Alloc>& y){
			return ( !(x == y) );
		};
	template <class T, class Alloc>
		bool operator<  (const vector<T,Alloc>& x, const vector<T,Alloc>& y){
			return ( ft::lexicographical_compare(x.begin(), x.end(), y.begin(), y.end()) );
		};
	template <class T, class Alloc>
		bool operator<= (const vector<T,Alloc>& x, const vector<T,Alloc>& y){
			return( !(y < x) );
		};
	template <class T, class Alloc>
		bool operator>  (const vector<T,Alloc>& x, const vector<T,Alloc>& y){
			return ( y < x );
		};
	template <class T, class Alloc>
		bool operator>= (const vector<T,Alloc>& x, const vector<T,Alloc>& y){
			return ( !(x < y) );
		};

	template <class T, class Alloc>
		void swap (vector<T, Alloc>& x, vector<T, Alloc>& y){
			x.swap(y);
		}
}

#endif
