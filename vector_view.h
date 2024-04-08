#pragma once

#include <vector>

#if (defined(_MSVC_LANG) && _MSVC_LANG >= 202002L) || __cplusplus >= 202002L
#define _CONSTEXPR constexpr
#else
#define _CONSTEXPR
#endif

template<typename T, typename Alloc = std::allocator<T>>
class vector_view {

public:
	using iterator = typename std::vector<T, Alloc>::iterator;
	using const_iterator = typename std::vector<T, Alloc>::const_iterator;
	using value_type = T;
	using allocator_type = Alloc;
	using size_type = typename std::vector<T, Alloc>::size_type;
	using difference_type = typename  std::vector<T, Alloc>::difference_type;
	using reference = value_type&;
	using const_reference = const value_type&;
	using pointer = std::vector<T, Alloc>::pointer;
	using const_pointer = typename std::vector<T, Alloc>::const_pointer;
	using reverse_iterator = std::reverse_iterator<iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;

	_CONSTEXPR vector_view(std::vector<T, Alloc>& rVector) :Vector(rVector), itBegin(rVector.begin()), itEnd(rVector.end()) {}
	_CONSTEXPR vector_view(std::vector<T, Alloc>& rVector, const iterator& itBegin, const iterator& itEnd) :Vector(rVector), itBegin(itBegin), itEnd(itEnd) {}

	template<typename... TArgs>
	_CONSTEXPR void emplace_back(TArgs&&... Args) {
		const auto lSize = size();
		itEnd = Vector.emplace(itEnd, std::forward<TArgs>(Args)...) + 1;
		itBegin = itEnd - (lSize + 1);
		
	}

	_CONSTEXPR iterator insert(const_iterator it, const T& value) {
		const auto Size = size();
		const auto Offset = it - itBegin;
		it = Vector.insert(it, value);
		itBegin = it - Offset;
		itEnd = itBegin + (Size + 1);
		return it;
	}
	
	_CONSTEXPR iterator insert(const_iterator it, T&& value) {
		const auto Size = size();
		const auto Offset = it - itBegin;
		it = Vector.insert(it, std::move(value));
		itBegin = it - Offset;
		itEnd = itBegin + (Size + 1);
		return it;
	}
	
	_CONSTEXPR iterator insert(const_iterator it, size_type count, const T& value) {
		const auto Size = size();
		const auto Offset = it - itBegin;
		it = Vector.insert(it, count, value);
		itBegin = it - Offset;
		itEnd = itBegin + (Size + count);
		return it;
	}

	_CONSTEXPR void reserve(size_t NewSize) {
		if (NewSize > size()) {
			const auto Offset = itBegin - Vector.begin();
			const auto OldSize = size();
			Vector.reserve(NewSize - size() + Vector.size());
			itBegin = Vector.begin() + Offset;
			itEnd = itBegin + OldSize;
		}
	}

	_CONSTEXPR void resize(size_t NewSize) {
		if (NewSize > size()) {
			reserve(NewSize);
			insert(itEnd, NewSize - size(), T());
		}
		if (NewSize < size()) {
			erase(itEnd - (size() - NewSize), itEnd);
		}
	}

	_CONSTEXPR size_t size() const noexcept {
		return itEnd - itBegin;
	}
	_CONSTEXPR bool empty() const noexcept {
		return size() == 0;
	}

	_CONSTEXPR T& operator[](size_t Index) noexcept {
		
		return *(itBegin + Index);
	}

	_CONSTEXPR const T& operator[](size_t Index) const noexcept {
		return *(itBegin + Index);
	}

	_CONSTEXPR T& back() noexcept {
		return operator[](size() - 1);
	}

	_CONSTEXPR const T& back() const noexcept {
		return operator[](size() - 1);
	}

	
	_CONSTEXPR iterator begin() noexcept {
		return itBegin;
	}
	_CONSTEXPR iterator end() noexcept {
		return itEnd;
	}

	_CONSTEXPR const_iterator begin() const noexcept {
		return itBegin;
	}
	_CONSTEXPR const_iterator end() const noexcept {
		return itEnd;
	}

	_CONSTEXPR const_iterator cbegin() const noexcept {
		return itBegin;
	}
	_CONSTEXPR const_iterator cend() const noexcept {
		return itEnd;
	}

	_CONSTEXPR reverse_iterator rbegin() noexcept {
		return itEnd;
	}
	_CONSTEXPR reverse_iterator rend() noexcept {
		return itBegin;
	}

	_CONSTEXPR const_reverse_iterator rbegin() const noexcept {
		return itEnd;
	}
	_CONSTEXPR const_reverse_iterator rend() const noexcept {
		return itBegin;
	}

	_CONSTEXPR const_reverse_iterator crbegin() const noexcept {
		return itEnd;
	}
	_CONSTEXPR const_reverse_iterator crend() const noexcept {
		return itBegin;
	}

	template<typename... TArgs>
	_CONSTEXPR auto emplace(iterator it, TArgs&&... Args) {
		const auto Size = size();
		const auto Offset = it - itBegin;
		it = Vector.emplace(it, std::forward<TArgs>(Args)...);
		itBegin = it - Offset;
		itEnd = itBegin + (Size + 1);
		return it;
	}

	_CONSTEXPR auto erase(iterator it) noexcept {
		const auto Size = size();
		const auto Offset = it - itBegin;
		it = Vector.erase(it);
		itBegin = it - Offset;
		itEnd = itBegin + (Size - 1);
		return it;
	}

	
	_CONSTEXPR auto erase(iterator itFirst, const iterator& itLast) noexcept {
		const auto Size = size();
		const auto Offset = itFirst - itBegin;
		const size_t NumErased = std::distance(itFirst, itLast);
		itFirst = Vector.erase(itFirst, itLast);
		itBegin = itFirst - Offset;
		itEnd = itBegin + (Size - NumErased);
		return itFirst;
	}

	_CONSTEXPR void pop_back() noexcept {
		erase(itEnd);
	}
	

private:
	std::vector<T, Alloc>& Vector;
	iterator itBegin, itEnd;
};



template<typename T, typename Alloc = std::allocator<T>>
class const_vector_view {
public:
	using const_iterator = typename std::vector<T, Alloc>::const_iterator;
	using value_type = T;
	using allocator_type = Alloc;
	using size_type = typename std::vector<T, Alloc>::size_type;
	using difference_type = typename std::vector<T, Alloc>::difference_type;
	using const_reference = const value_type&;
	using const_pointer = typename std::vector<T, Alloc>::const_pointer;
	using const_reverse_iterator = typename std::reverse_iterator<const_iterator>;

	const_vector_view(const const_iterator& itBegin, const const_iterator& itEnd) : itBegin(itBegin), itEnd(itEnd) {}
	const_vector_view(const std::vector<T, Alloc>& rVector) : itBegin(rVector.begin()), itEnd(rVector.end()) {}
	const_vector_view(const vector_view<T, Alloc>& rVectorView) : itBegin(rVectorView.begin()), itEnd(rVectorView.end()) {}

	
	_CONSTEXPR size_t size() const noexcept {
		return itEnd - itBegin;
	}
	_CONSTEXPR bool empty() const noexcept {
		return size() == 0;
	}

	
	_CONSTEXPR const T& operator[](size_t Index) const noexcept {
		return *(itBegin + Index);
	}

	_CONSTEXPR const T& back() const noexcept {
		return operator[](size() - 1);
	}


	_CONSTEXPR const_iterator begin() const noexcept {
		return itBegin;
	}
	_CONSTEXPR const_iterator end() const noexcept {
		return itEnd;
	}

	_CONSTEXPR const_iterator cbegin() const noexcept {
		return itBegin;
	}
	_CONSTEXPR const_iterator cend() const noexcept {
		return itEnd;
	}

	_CONSTEXPR const_reverse_iterator rbegin() const noexcept {
		return itEnd;
	}
	_CONSTEXPR const_reverse_iterator rend() const noexcept {
		return itBegin;
	}

	_CONSTEXPR const_reverse_iterator crbegin() const noexcept {
		return itEnd;
	}
	_CONSTEXPR const_reverse_iterator crend() const noexcept {
		return itBegin;
	}



private:
	const_iterator itBegin, itEnd;
};
