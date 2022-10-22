#ifndef VECTOR_HPP_
#define VECTOR_HPP_
# include <memory>
# include <algorithm>
# include <stdexcept>
# include "iterator.hpp"
# include "type_traits.hpp"
# include "algorithm.hpp"

namespace ft
{

template < class T, class Alloc = std::allocator<T> >
class vector {
public:
	typedef T												value_type;
	typedef typename Alloc::template rebind<T>::other		allocator_type;
	typedef typename allocator_type::reference				reference;
	typedef typename allocator_type::const_reference		const_reference;
	typedef typename allocator_type::pointer				pointer;
	typedef typename allocator_type::const_pointer			const_pointer;
	typedef pointer											iterator;
	typedef const_pointer									const_iterator;
	typedef typename ft::reverse_iterator<iterator> 		reverse_iterator;
	typedef typename ft::reverse_iterator<const_iterator>	const_reverse_iterator;
	typedef size_t											size_type;
	typedef ptrdiff_t										difference_type;

	explicit vector(const allocator_type& a = allocator_type()) :
	_allocator(a), _start(0), _finish(0), _end_of_storage(0) {}

	explicit vector(size_type n, const T& value = value_type(),
	const allocator_type& a = allocator_type()) : _allocator(a) {
		_start = _allocator.allocate(n);
		_end_of_storage = _start + n;
		iterator i = _start;
		for (; n > 0; --n, ++i) {
			_allocator.construct(i, value);
		}
		_finish = i;
	}

	template <class InputIterator>
	vector(InputIterator first, InputIterator last,
	const allocator_type& a = allocator_type(),
	typename ft::enable_if<!ft::is_integral<InputIterator>::value, InputIterator>::type* = 0) :
	_allocator(a) {
		size_type n = std::distance(first, last);
		_start = _allocator.allocate(n);
		_end_of_storage = _start + n;
		pointer i = _start;
		for (; first != last; ++i, ++first) {
			_allocator.construct(i, *first);
		}
		_finish = i;
	}

	vector(const vector<T,Alloc>& x) : _allocator(x._allocator)
	{
		size_type n = x.size();
		_start = _allocator.allocate(n);
		_end_of_storage = _start + n;
		pointer i = _start;
		pointer j = x._start;
		for (;n > 0; --n, ++i, ++j) {
			_allocator.construct(i, *j);
		}
		_finish = i;
	}

	~vector() {
		for (iterator i = _start; i != _finish; ++i) {
			_allocator.destroy(i);
		}
		_allocator.deallocate(_start, _end_of_storage - _start);
	}

	vector<T,Alloc>& operator=(const vector<T,Alloc>& x)
	{
		if (&x != this) {
			const size_type xlen = x.size();
			if (xlen > capacity()) {
				pointer new_start = _allocator.allocate(xlen);
				pointer i = new_start;
				pointer j = const_cast<pointer>(x.begin());
				for (; j != x.end(); ++i, ++j) {
					_allocator.construct(i, *j);
				}
				for (iterator i = _start; i != _finish; ++i) {
					_allocator.destroy(i);
				}
				_allocator.deallocate(_start, _end_of_storage - _start);
				_start = new_start;
				_end_of_storage = _start + xlen;
			}
			else if (size() >= xlen) {
				iterator i(std::copy(x.begin(), x.end(), begin()));
				for (; i != end(); ++i) {
					_allocator.destroy(i);
				}
			}
			else {
				std::copy(x.begin(), x.begin() + size(), _start);
				iterator i = _finish;
				iterator j = const_cast<pointer>(x.begin()) + size();
				for (; j != x.end(); ++i, ++j) {
					_allocator.construct(i, *j);
				}
			}
			_finish = _start + xlen;
		}
		return *this;
	}

	template <class InputIterator>
	void assign(InputIterator first, InputIterator last,
		typename ft::enable_if<!ft::is_integral<InputIterator>::value, InputIterator>::type* = 0) {
		erase(begin(), end());
		insert(begin(), first, last);
	}
	void assign(size_type n, const T& t) {
		erase(begin(), end());
		insert(begin(), n, t);
	}

	allocator_type get_allocator() const { return allocator_type(); }

	// Iterators:
	iterator				begin() { return iterator(_start); }
	const_iterator			begin() const { return const_iterator(_start); }
	iterator				end() { return iterator(_finish); }
	const_iterator			end() const { return const_iterator(_finish); }
	reverse_iterator		rbegin() { return reverse_iterator(end()); }
	const_reverse_iterator	rbegin() const { return const_reverse_iterator(end()); }
	reverse_iterator		rend() { return reverse_iterator(begin()); }
	const_reverse_iterator	rend() const { return const_reverse_iterator(begin()); }

	// Capacity:
	size_type size() const { return size_type(end() - begin()); }
	size_type max_size() const { return _allocator.max_size(); }

	void resize (size_type n, value_type val = value_type()) {
		if (n > size())
			insert(end(), n - size(), val);
		else if (n < size())
			erase(begin() + n, end());
	}

	size_type capacity() const
	{ return size_type(const_iterator(this->_end_of_storage) - begin()); }

	bool empty() const { return begin() == end(); }

	void reserve (size_type n) {
		if (capacity() < n) {
			if (n > max_size()) {
				throw std::length_error("vector");
			}
			const size_type old_size = size();
			pointer new_start(_allocator.allocate(n));
			std::copy(_start, _finish, new_start);
			for (iterator i = _start; i != _finish; ++i) {
				_allocator.destroy(i);
			}
			_allocator.deallocate(_start, _end_of_storage - _start);
			this->_start = new_start;
			this->_finish = new_start + old_size;
			this->_end_of_storage = new_start + n;
		}
	}

	// Element access:
	reference		operator[] (size_type n) { return *(begin() + n); }
	const_reference	operator[] (size_type n) const { return *(begin() + n); }

	reference		at (size_type n)
		{ _range_check(n); return (*this)[n]; }
	const_reference	at (size_type n) const
		{ _range_check(n); return (*this)[n]; }
	reference		front()
		{ return *begin(); }
	const_reference	front() const
		{ return *begin(); }
	reference		back()
		{ return *(end() - 1); }
	const_reference	back() const
		{ return *(end() - 1); }

	void push_back(const T& x) {
		if (this->_finish != this->_end_of_storage) {
			this->_allocator.construct(this->_finish, x);
			++this->_finish;
		}
		else {
			const size_type old_size = size();
			const size_type len = old_size != 0 ? 2 * old_size : 1;
			iterator new_start(_allocator.allocate(len));
			iterator new_finish(new_start);
			iterator i = new_start;
			iterator j = _start;
			for (;j != _finish; ++i, ++j) {
				_allocator.construct(i, *j);
			}
			new_finish = i;
			_allocator.construct(new_finish, x);
			++new_finish;
			for (iterator i = _start; i != _finish; ++i) {
				_allocator.destroy(i);
			}
			_allocator.deallocate(_start, _end_of_storage - _start);
			_start = new_start;
			_finish = new_finish;
			_end_of_storage = new_start + len;
		}
	}

	void pop_back() {
		--this->_finish;
		this->_allocator.destroy(this->_finish);
	}

	iterator insert(iterator position, const T& x) {
		size_type n = position - begin();
		if (_finish != _end_of_storage) {
			if (position == end()) {
				this->_allocator.construct(this->_finish, x);
				++_finish;
			}
			else {
				_allocator.construct(_finish, *(_finish - 1));
				++_finish;
				value_type x_copy = x;
				iterator i = _finish - 2;
				iterator j = _finish - 1;
				while (i != position) {
					*--j = *--i;
				}
				*position = x_copy;
			}
		}
		else {
			const size_type old_size = size();
			const size_type len = old_size != 0 ? 2 * old_size : 1;
			iterator new_start(_allocator.allocate(len));
			iterator new_finish(new_start);
			iterator i = new_start;
			iterator j = _start;
			for (; j != position; ++i, ++j) {
				_allocator.construct(i, *j);
			}
			new_finish = i;
			_allocator.construct(new_finish, x);
			++new_finish;
			i = new_finish;
			j = position;
			for (; j != _finish; ++i, ++j) {
				_allocator.construct(i, *j);
			}
			new_finish = i;
			for (iterator i = _start; i != _finish; ++i) {
				_allocator.destroy(i);
			}
			_allocator.deallocate(_start, _end_of_storage - _start);
			_start = new_start;
			_finish = new_finish;
			_end_of_storage = new_start + len;
		}
		return begin() + n;
	}

	void insert(iterator position, size_type n, const T& x) {
		if (n == 0) {
			return ;
		}
		if (size_type(_end_of_storage - _finish) >= n) {
			const size_type _elems_after = end() - position;
			iterator _old_finish(_finish);
			if (_elems_after > n) {
				iterator i = _finish;
				iterator j = _finish - n;
				for (; j != _finish; ++i, ++j) {
					_allocator.construct(i, *j);
				}
				_finish += n;
				i = _old_finish - n;
				j = _old_finish;
				while (i != position) {
					*--j = *--i;
				}
				for (; n > 0; --n, ++position) {
					value_type x_copy = x;
					*position = x_copy;
				}
			}
			else {
				//need check
				iterator i = _finish;
				n -= _elems_after;
				for (; n > 0; ++i, --n) {
					value_type x_copy = x;
					_allocator.construct(i, x_copy);
				}
				iterator j = position;
				for (; j != _old_finish; ++i, ++j) {
					_allocator.construct(i, *j);
				}
				_finish = i;
				for (; position != _old_finish; ++position) {
					value_type x_copy = x;
					*position = x_copy;
				}
			}
		}
		else {
			const size_type old_size = size();
			const size_type len = old_size + std::max(old_size, n);
			iterator new_start(_allocator.allocate(len));
			iterator new_finish(new_start);
			iterator i = new_start;
			iterator j = _start;
			for (; j != position; ++i, ++j) {
				_allocator.construct(i, *j);
			}
			new_finish = i;
			for (; n > 0; ++i, --n) {
				_allocator.construct(i, x);
			}
			new_finish = i;
			j = position;
			for (; j != _finish; ++i, ++j) {
				_allocator.construct(i, *j);
			}
			new_finish = i;
			for (iterator i = _start; i != _finish; ++i) {
				_allocator.destroy(i);
			}
			_allocator.deallocate(_start, _end_of_storage - _start);
			_start = new_start;
			_finish = new_finish;
			_end_of_storage = new_start + len;
		}
	}

	template <class InputIterator>
	void insert(iterator position, InputIterator first, InputIterator last,
		typename ft::enable_if<!ft::is_integral<InputIterator>::value, InputIterator>::type* = 0) {
		for(; first != last; ++first) {
			position = insert(position, *first);
			++position;
		}
	}

	iterator erase(iterator position) {
		std::copy(position + 1, end(), position);
		--_finish;
		_allocator.destroy(_finish);
		return position;
	}

	iterator erase(iterator first, iterator last) {
		iterator i(std::copy(last, end(), first));
		for (; i != end(); ++i) {
			_allocator.destroy(i);
		}
		_finish = _finish - (last - first);
		return first;
	}

	void swap(vector<T,Alloc>& x) {
		std::swap(this->_start, x._start);
		std::swap(this->_finish, x._finish);
		std::swap(this->_end_of_storage, x._end_of_storage);
	}

	void clear() {
		erase(begin(), end());
	}

protected:
	allocator_type _allocator;
	T*	_start;
	T*	_finish;
	T*	_end_of_storage;

private:

	void	_range_check(size_type n) const {
		if (n >= this->size())
			throw std::out_of_range("vector");
	}

};

template <class T, class Alloc>
bool
operator==(const vector<T,Alloc>& x, const vector<T,Alloc>& y)
{ return (x.size() == y.size() && ft::equal(x.begin(), x.end(), y.begin())); }

template <class T, class Alloc>
bool
operator< (const vector<T,Alloc>& x, const vector<T,Alloc>& y)
{ return ft::lexicographical_compare(x.begin(), x.end(), y.begin(), y.end()); }

template <class T, class Alloc>
bool
operator!=(const vector<T,Alloc>& x, const vector<T,Alloc>& y)
{ return !(x == y); }

template <class T, class Alloc>
bool
operator> (const vector<T,Alloc>& x, const vector<T,Alloc>& y)
{ return y < x; }

template <class T, class Alloc>
bool
operator>=(const vector<T,Alloc>& x, const vector<T,Alloc>& y)
{ return !(x < y); }

template <class T, class Alloc>
bool
operator<=(const vector<T,Alloc>& x, const vector<T,Alloc>& y)
{ return !(y < x); }

template <class T, class Alloc>
void swap(vector<T,Alloc>& x, vector<T,Alloc>& y) { x.swap(y); }

} // namespace ft

#endif  // VECTOR_HPP_
