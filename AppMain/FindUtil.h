/******************************************************************************
 *  ��Ȩ���У�C��2012-2016                                                    *
 *  ��������Ȩ����                                                            *
 *****************************************************************************
 *  ���� : ����
 *  �汾 : 1.0
 *****************************************************************************/
/*  �޸ļ�¼: 
      ����       �汾    �޸���             �޸�����
    --------------------------------------------------------------------------
******************************************************************************/
#ifndef FindUtil_h__
#define FindUtil_h__

#include <vector>
#include <set>

namespace MyWeb
{
    template<class T, class ForwardIt, class Comp>
    std::vector<std::pair<T, int>> AdjacentFind(ForwardIt first, ForwardIt last, Comp comp, const T& value)
    {
        std::vector<std::pair<T, int>> ret;
        if (first == last) {
            return ret;
        }

        size_t pos = 0;
        ForwardIt it = first;
        ForwardIt markedIt = first;
        for (; it != last; ++pos, ++it) {
            if (comp(*it, value)) {
                ret.emplace_back(std::make_pair(*it, std::distance(markedIt, it)));
                markedIt = it;
            }
        }
        return ret;
    }

    template<class T, class ForwardIt, class Comp>
    std::vector<std::pair<T, int>> AdjacentFind(ForwardIt first, ForwardIt last, Comp comp, const std::set<int>& values)
    {
        std::vector<std::pair<T, int>> ret;
        if (first == last) {
            return ret;
        }

        size_t pos = 0;
        ForwardIt it = first;
        ForwardIt markedIt = first;
        for (; it != last; ++pos, ++it) {
            if (comp(*it, values)) {
                ret.emplace_back(std::make_pair(*it, std::distance(markedIt, it)));
                markedIt = it;
            }
        }
        return ret;
    }
}

#endif // FindUtil_h__