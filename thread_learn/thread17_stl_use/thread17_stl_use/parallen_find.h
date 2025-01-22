#pragma once
#include "join_thread.h"
template<typename Iterator, typename MatchType>
Iterator parallel_find(Iterator first, Iterator last, MatchType match)
{
    struct find_element    //⇽-- - 1
    {
        void operator()(Iterator begin, Iterator end,
            MatchType match,
            std::promise<Iterator>* result,
            std::atomic<bool>* done_flag)
        {
            try
            {
                for (; (begin != end) && !done_flag->load(); ++begin)    //⇽-- - 2
                {
                    if (*begin == match)
                    {
                        result->set_value(begin);    //⇽-- - 3
                        done_flag->store(true);    //⇽-- - 4
                        return;
                    }
                }
            }
            catch (...)    //⇽-- - 5
            {
                try
                {
                    result->set_exception(std::current_exception());    //⇽-- - 6
                    done_flag->store(true);
                }
                catch (...)    //⇽-- - 7
                {
                }
            }
        }
    };
    unsigned long const length = std::distance(first, last);
    if (!length)
        return last;
    unsigned long const min_per_thread = 25;
    unsigned long const max_threads = (length + min_per_thread - 1) / min_per_thread;
    unsigned long const hardware_threads = std::thread::hardware_concurrency();
    unsigned long const num_threads = std::min(hardware_threads != 0 ? hardware_threads : 2, max_threads);
    unsigned long const block_size = length / num_threads;
    std::promise<Iterator> result;    //⇽-- - 8
    std::atomic<bool> done_flag(false);     //⇽-- - 9
    std::vector<std::thread> threads(num_threads - 1); //⇽-- - 10
    {
        join_threads joiner(threads);
        Iterator block_start = first;
        for (unsigned long i = 0; i < (num_threads - 1); ++i)
        {
            Iterator block_end = block_start;
            std::advance(block_end, block_size);
            // ⇽-- - 11
            threads[i] = std::thread(find_element(), block_start, block_end, match, &result, &done_flag);
            block_start = block_end;
        }
        // ⇽-- - 12
        find_element()(block_start, last, match, &result, &done_flag);
    }

    // ⇽-- - 13
    if (!done_flag.load())
    {
        return last;
    }
    //⇽-- - 14
    return result.get_future().get();
}


template<typename Iterator, typename MatchType>
Iterator parallel_find_impl(Iterator first, Iterator last, MatchType match,
    std::atomic<bool>& done)   // ⇽-- - 1
{
    try
    {
        unsigned long const length = std::distance(first, last);
        unsigned long const min_per_thread = 25;   // ⇽-- - 2
        if (length < (2 * min_per_thread))    //⇽-- - 3
        {
            for (; (first != last) && !done.load(); ++first)     //⇽-- - 4
            {
                if (*first == match)
                {
                    done = true;    //⇽-- - 5
                    return first;
                }
            }
            return last;    //⇽-- - 6
        }
        else
        {
            //⇽-- - 7
            Iterator const mid_point = first + (length / 2);
            //⇽-- - 8
            std::future<Iterator> async_result = std::async(&parallel_find_impl<Iterator, MatchType>,
                mid_point, last, match, std::ref(done));
            //⇽-- - 9
            Iterator const direct_result = parallel_find_impl(first, mid_point, match, done);
            //⇽-- - 10
            return (direct_result == mid_point) ? async_result.get() : direct_result;
        }
    }
    catch (...)
    {
        // ⇽-- - 11
        done = true;
        throw;
    }
}
template<typename Iterator, typename MatchType>
Iterator parallel_find_async(Iterator first, Iterator last, MatchType match)
{
    std::atomic<bool> done(false);
    //⇽-- - 12
    return parallel_find_impl(first, last, match, done);
}