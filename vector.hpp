/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jisokang <jisokang@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/21 19:25:17 by jisokang          #+#    #+#             */
/*   Updated: 2022/10/06 20:50:10 by jisokang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//vector<bool> 전문화를 수행할 필요가 없습니다. ->

#ifndef VECTOR_HPP
# define VECTOR_HPP

# include <memory>
# include <stdexcept>
# include "iterator_traits.hpp"
# include "iterator.hpp"
/**
 * @brief
 *
 */
namespace ft
{
	//template <class T, class Allocator = std::allocator<T> -> 내가 만든 얼록케이터라는것을 확실하게 하기 위해서
	template <class T, class Alloc = std::allocator<T> >			//왜 Allocator가 아니라 Alloc으로 하는 걸까?
	class vector
	{
	public:
		//types:
		typedef T												value_type;
		typedef typename	Alloc::template rebind<T>::other	allocator_type; //안써도됌 ㅋㅋㅋ크루 삥뽕 근데 설명은 해야함.. 젠장.
		//typedef Allocator								allocator_type;
		typedef typename	allocate_type::reference			reference;
		typedef typename	allocate_type::const_reference		const_reference;
		typedef typename	allocate_type::pointer				pointer;
		typedef typename	allocate_type::const_pointer		const_pointer;
	//ISO 14882_1998 23.1 implementation-defined ==============================
		typedef pointer											iterator;
		typedef const_pointer									const_iterator;
		typedef size_t											size_type;
		typedef ptrdiff_t										difference_type;
	//define end ==============================================================
		typedef ft::reverse_iterator<iterator>					reverse_iterator;
		typedef ft::reverse_iterator<const_iterator>			const_reverse_iterator;

		//23.2.4.1 construct/copy/destroy
		//explicit: 생성자 앞에 explicit 키워드를 붙여주면 변환 생성자의 무작위 호출을 막고 명확성을 높여준다.
		//explicit	vector(const Allocator& = Allocator())
		explicit	vector(const allocator_type& alloc = allocator_type())
			: _alloc(alloc), _start(0), _finish(0), _end_storage(0)
			{};
		//왜 allocator로 alloc을 따로 하는거지?
		//explicit	vector(size_type n, const T& value = T(), const Allocator& = Allocator())
		explicit	vector(size_type n, const T& value = T(), const allocator_type& alloc = Allocator())
			: _alloc(alloc), _start(0), _finish(0), _end_storage(0)
			{
				_start = _alloc.allocate( n );
				_end_storage = _start + n;
				iterator i = _start;
				while (n--)
				{
					_alloc.construct( i, value);
					i++;
				}
				_finish = i;
			};

		//need enable_if here! =========================================
		template <class InputIterator>
			vector(InputIterator first, InputIterator last, const Allocator& = Allocator());
		//==============================================================

		vector(const vector<T, Alloc>& x) : _alloc(x._alloc)
		{
			size_type n = x.size();
			_start = _alloc.allocate( n );
			_end_storage = _start + n;
			iterator i = _start;
			iterator j = x._start;
			while (n--)
			{
				_alloc.construct( i, *j );	//이건 왜 *j 일까?
				i++;
				j++;
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
			_allocator.deallocate(_start, _end_storage - _start);
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
					iterator it = _start
					while (it != _finish)
					{
						_alloc.destory(it);
						it++;
					}
					_alloc.deallocate(_start, _end_storage - _start);
					_start = new_start;
					_end_storage = _start + x_len;
				}
				//내가 x보다 크거나 같으면
				else if (size() >= x_len)
				{
					iterator it(std::copy(x.begin(), x.end(), begin()));
					while (it != end())
					{
						_alloc.destory(it);
						it++;
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

		//need enable_if here! =========================================
		template <class InputIterator>
			void assign(InputIterator first, InputIterator last)
			{

			};
		//==============================================================
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
		size_type				size() const{
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
		void					resize(size_type sz, value_type c = value_type()){
			if (n > size()){
				insert(end(), sz - size(), c);
			}
			else if (n < size()){
				erase(begin() + sz, end());
			}
			else{
				/* Same, Nothing to do*/
			}
		};
		size_type				capacity() const
		{
			return ( size_type(const_iterator(_end_storage) - begin() );
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
		void					reverse(size_type n)
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
				_alloc.dellocate(_start, _end_storage - _start);
				_start = new_start;
				_finish = new_start + old_size;
				_end_storage = new_start + n;
			}
		};

		//element access
		reference				operator[](size_type n){
			return ( *(begin() + n) );
		};
		const_reference			operator[](size_type n) const{
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
			return ( *begin() );		//왜 주소를 줄까?
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
		void					pop_back(){
			_finish--;
			_alloc.destroy(_finish);
		};
		iterator				insert(iterator position, const T& x);
		void					insert(iterator position, size_type n, const T& x);
		template <class InputIterator>
			void				insert(iterator position, InputIterator first, InputIterator last);
		iterator				erase(iterator position);
		iterator				erase(iterator first, iterator last);
		void					swap(vector<T, Allocator>&){
			std::swap(_start, x._start);
			std::swap(_finish, x._finish);
			std::swap(_end_storage, x._end_storage);
		};
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

}

#endif
