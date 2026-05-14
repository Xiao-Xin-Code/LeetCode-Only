#pragma once

#include <algorithm>
#include <iterator>
#include <functional>

#pragma region Pop Sort

template<typename _Ranlt, typename _Pr>
void popSort(_Ranlt _First, _Ranlt _Last, _Pr _Pred) {
	//TODO: security check
	_Pop_Sort_unchecked(_First, _Last, _Pred);
}

template<typename _Ranlt>
void popSort(_Ranlt _First, _Ranlt _Last) {
	popSort(_First, _Last, std::less<>{});
}

template<typename _Ranlt, typename _Pr>
void _Pop_Sort_unchecked(_Ranlt _First, _Ranlt _Last, _Pr _Pred) {
	_Ranlt _End = _Last;
	bool hasSwap = true;
	while (_End > _First && hasSwap) {
		hasSwap = false;
		for (_Ranlt it = _First;it + 1 != _End;++it) {
			if (_Pred(*(it + 1), *it)) {
				hasSwap = true;
				std::swap(*it, *(it + 1));
			}
		}
		--_End;
	}
}

#pragma endregion

#pragma region Insert Sort

template<typename _Ranlt, typename _Pr>
void insertSort(_Ranlt _First, _Ranlt _Last, _Pr _Pred) {
	_Insert_Sort_unchecked(_First, _Last, _Pred);
}

template<typename _Ranlt>
void insertSort(_Ranlt _First, _Ranlt _Last) {
	insertSort(_First, _Last, std::less<>{});
}

template<typename _Ranlt, typename _Pr>
void _Insert_Sort_unchecked(_Ranlt _First, _Ranlt _Last, _Pr _Pred) {
	for (_Ranlt it = _First;it != _Last;++it) {
		auto temp = std::move(*it);
		_Ranlt _Temp = it;
		while (_Temp > _First && _Pred(temp, *(_Temp - 1))) {
			*_Temp = std::move(*(_Temp - 1));
			--_Temp;
		}
		*_Temp = std::move(temp);
	}
}

#pragma endregion

#pragma region Hill Sort

template<typename _Ranlt,typename _Pr>
void hillSort(_Ranlt _First, _Ranlt _Last, _Pr _Pred) {
	_Hill_Sort_unchecked(_First, _Last, _Pred);
}

template<typename _Ranlt>
void hillSort(_Ranlt _First, _Ranlt _Last) {
	hillSort(_First, _Last, std::less<>{});
}

template<typename _Ranlt, typename _Pr>
void _Hill_Sort_unchecked(_Ranlt _First, _Ranlt _Last, _Pr _Pred) {
	auto space = std::distance(_First, _Last) / 2;
	while(space > 0){
		for(_Ranlt it = _First + space; it!=_Last;++it){
			auto temp = std::move(*it);
			_Ranlt _Temp = it;
			while(_Temp >= _First + space && _Pred(temp, *(_Temp - space))){
				*_Temp = std::move(*(_Temp - space));
				_Temp -= space;
			}
			*_Temp = std::move(temp);
		}
		space /= 2;
	}
}

#pragma endregion

#pragma region Quick Sort

template<typename _Ranlt,typename _Pr>
void quickSort(_Ranlt _First, _Ranlt _Last, _Pr _Pred) {
	_Quick_Sort_unchecked(_First, _Last, _Pred);
}

template<typename _Ranlt>
void quickSort(_Ranlt _First, _Ranlt _Last) {
	quickSort(_First, _Last, std::less<>{});
}

template<typename _Ranlt, typename _Pr>
void _Quick_Sort_unchecked(_Ranlt _First, _Ranlt _Last, _Pr _Pred) {
	if (_First + 1 >= _Last) return;
	_Ranlt _Low = _First;
	_Ranlt _High = _Last - 1;
	auto _Pivot = *_First;
	while (_Low < _High) {
		while (_Low < _High && _Pred(_Pivot, *_High)) --_High;
		*_Low = *_High; ++_Low;
		while (_Low < _High && _Pred(*_Low, _Pivot)) ++_Low;
		*_High = *_Low; --_High;
	}
	*_Low = _Pivot;
	_Quick_Sort_unchecked(_First, _Low, _Pred);
	_Quick_Sort_unchecked(_Low + 1, _Last, _Pred);
}

#pragma endregion

#pragma region Select Sort

template<typename _Ranlt, typename _Pr>
void selectSort(_Ranlt _First, _Ranlt _Last, _Pr _Pred) {
	_Select_Sort_unchecked(_First, _Last, _Pred);
}

template<typename _Ranlt>
void selectSort(_Ranlt _First, _Ranlt _Last) {
	selectSort(_First, _Last, std::less<>{});
}

template<typename _Ranlt, typename _Pr>
void _Select_Sort_unchecked(_Ranlt _First, _Ranlt _Last, _Pr _Pred) {
	for (_Ranlt it = _First;it != _Last;++it) {
		_Ranlt _Min = it;
		for (_Ranlt op = it + 1;op != _Last;++op) {
			if (_Pred(*op, *_Min)) {
				_Min = op;
			}
		}
		if (_Min != it) std::swap(*it, *_Min);
	}
}

#pragma endregion
