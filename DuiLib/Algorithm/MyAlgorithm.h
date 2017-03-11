/******************************************************************************
 *  ��Ȩ���У�C��2012-2016													*
 *  ��������Ȩ����                                                            *
 *****************************************************************************
 *  ���� : ����
 *  �汾 : 1.0
 *****************************************************************************/
/*  �޸ļ�¼: 
      ����       �汾    �޸���             �޸�����
    --------------------------------------------------------------------------
******************************************************************************/
#pragma once

#include <utility>

namespace MyAlgorithm
{
	// ��չminmax_element
	template<class ForwardIt, class CompareLess, class CompareGreater>
	std::pair<ForwardIt, ForwardIt> 
		my_minmax_element(ForwardIt first, ForwardIt last, CompareLess compLess, CompareGreater compGreater)
	{
		std::pair<ForwardIt, ForwardIt> result(first, first);

		if (first == last) return result;
		if (++first == last) return result;

		if (compLess(*first, *result.first)) {
			result.first = first;
		} 

		if (compGreater(*first, *result.second)) {
			result.second = first;
		}
		while (++first != last) {
			ForwardIt i = first;
			if (++first == last) {
				if (compLess(*i, *result.first)) result.first = i;
				if (compGreater(*i, *result.second)) result.second = i;
				break;
			} else {
				if (compLess(*first, *i)) {
					if (compLess(*first, *result.first)) result.first = first;
				} else {
					if (compLess(*i, *result.first)) result.first = i;
				}

				if (compGreater(*first, *i)){
					if (compGreater(*first, *result.second)) result.second = first;
				} else {
					if (compGreater(*i, *result.second)) result.second = i;
				}
			}
		}
		return result;
	}
}