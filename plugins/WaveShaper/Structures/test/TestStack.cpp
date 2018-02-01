#define BOOST_TEST_DYN_LINK

#ifdef STAND_ALONE
#define BOOST_TEST_MODULE Main
#endif

#include <boost/test/unit_test.hpp>
#include "../Stack.hpp"

BOOST_AUTO_TEST_SUITE(stack_suite)

BOOST_AUTO_TEST_CASE(stack_count_increment)
{
    spoonie::Stack<int> stack = spoonie::Stack<int>(3);

    stack.push(1);
    stack.push(2);
    stack.push(3);

    BOOST_REQUIRE(stack.getCount() == 3);
}

BOOST_AUTO_TEST_CASE(stack_count_decrement)
{
    spoonie::Stack<int> stack = spoonie::Stack<int>(3);
    
    stack.push(1);
    stack.push(2);
    stack.push(3);

    stack.pop();
    
    BOOST_REQUIRE(stack.getCount() == 2);
}

BOOST_AUTO_TEST_CASE(stack_pop_good_item)
{
    spoonie::Stack<int> stack = spoonie::Stack<int>(3);
    
    stack.push(1);
    stack.push(2);
    stack.push(3);

    BOOST_REQUIRE(stack.pop() == 3);
}

BOOST_AUTO_TEST_CASE(stack_peek_good_item)
{
    spoonie::Stack<int> stack = spoonie::Stack<int>(3);
    
    stack.push(1);
    stack.push(2);
    stack.push(3);

    BOOST_REQUIRE(stack.peek() == 3);
}

BOOST_AUTO_TEST_SUITE_END()
