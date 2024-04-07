#include <vector>

template<typename T, typename Alloc = std::allocator<T>>
class vector_view {

public:
	using iterator = std::vector<T, Alloc>::iterator;
	using const_iterator = std::vector<T, Alloc>::const_iterator;

	vector_view(std::vector<T, Alloc>& rVector) :Vector(rVector), itBegin(rVector.begin()), itEnd(rVector.end()) {}
	vector_view(std::vector<T, Alloc>& rVector, const iterator& itBegin, const iterator& itEnd) :Vector(rVector), itBegin(itBegin), itEnd(itEnd) {}

	template<typename... TArgs>
	void emplace_back(TArgs&&... Args) {
		const auto lSize = size();
		itEnd = Vector.emplace(itEnd, std::forward<TArgs>(Args)...) + 1;
		itBegin = itEnd - (lSize + 1);
		
	}

	void reserve(size_t NewSize) {
		if (NewSize > size()) {
			const auto Offset = itBegin - Vector.begin();
			const auto OldSize = size();
			Vector.reserve(NewSize - size() + Vector.size());
			itBegin = Vector.begin() + Offset;
			itEnd = itBegin + OldSize;
		}
	}

	void resize(size_t NewSize) {
		reserve(NewSize);
		while (NewSize > size()) {
			emplace_back();
		}
		while (NewSize < size()) {
			pop_back();
		}
	}

	size_t size() const {
		return itEnd - itBegin;
	}
	size_t empty() const {
		return size() == 0;
	}

	T& operator[](size_t Index) {
		
		return *(itBegin + Index);
	}

	const T& operator[](size_t Index) const {
		return *(itBegin + Index);
	}

	T& back() {
		return operator[](size() - 1);
	}

	const T& back() const {
		return operator[](size() - 1);
	}

	
	iterator begin() {
		return itBegin;
	}
	iterator end() {
		return itEnd;
	}

	const_iterator begin() const {
		return itBegin;
	}
	const_iterator end() const {
		return itEnd;
	}

	template<typename... TArgs>
	auto emplace(iterator it, TArgs&&... Args) {
		const auto Size = size();
		const auto Offset = it - itBegin;
		it = Vector.emplace(it, std::forward<TArgs>(Args)...);
		itBegin = it - Offset;
		itEnd = itBegin + (Size + 1);
		return it;
	}

	auto erase(iterator it) {
		const auto Size = size();
		const auto Offset = it - itBegin;
		it = Vector.erase(it);
		itBegin = it - Offset;
		itEnd = itBegin + (Size - 1);
		return it;
	}

	
	auto erase(iterator itFirst, const iterator& itLast) {
		const auto Size = size();
		const auto Offset = itFirst - itBegin;
		const size_t NumErased = std::distance(itFirst, itLast);
		itFirst = Vector.erase(itFirst, itLast);
		itBegin = itFirst - Offset;
		itEnd = itBegin + (Size - NumErased);
		return itFirst;
	}

	void pop_back() {
		erase(itEnd);
	}
	

private:
	std::vector<T, Alloc>& Vector;
	iterator itBegin, itEnd;
};



template<typename T, typename Alloc = std::allocator<T>>
class const_vector_view {

public:
	using const_iterator = std::vector<T, Alloc>::const_iterator;

	const_vector_view(const const_iterator& itBegin, const const_iterator& itEnd) : itBegin(itBegin), itEnd(itEnd) {}
	const_vector_view(const std::vector<T, Alloc>& rVector) : itBegin(rVector.begin()), itEnd(rVector.end()) {}

	
	size_t size() const {
		return itEnd - itBegin;
	}
	size_t empty() const {
		return size() == 0;
	}

	
	const T& operator[](size_t Index) const {
		return *(itBegin + Index);
	}

	const T& back() const {
		return operator[](size() - 1);
	}


	const_iterator begin() const {
		return itBegin;
	}
	const_iterator end() const {
		return itEnd;
	}





private:
	const_iterator itBegin, itEnd;
};
