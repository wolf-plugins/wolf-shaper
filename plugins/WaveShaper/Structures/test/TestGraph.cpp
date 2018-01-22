#define BOOST_TEST_DYN_LINK

#ifdef STAND_ALONE
#define BOOST_TEST_MODULE Main
#endif

#include <boost/test/unit_test.hpp>
#include "../Graph.hpp"

BOOST_AUTO_TEST_SUITE(graph_suite)

BOOST_AUTO_TEST_CASE(graph_add_nodes)
{
    spoonie::Graph graph = spoonie::Graph();
    
    graph.insertVertex(0.2f, 0.2f);
    graph.insertVertex(0.1f, 0.1f);
    graph.insertVertex(0.15f, 0.15f);
    graph.insertVertex(0.6f, 0.6f);
    
    BOOST_TEST(graph.getVertexAtIndex(0)->x == 0.0f);
    BOOST_TEST(graph.getVertexAtIndex(1)->x == 0.1f);
    BOOST_TEST(graph.getVertexAtIndex(2)->x == 0.15f);
    BOOST_TEST(graph.getVertexAtIndex(3)->x == 0.2f);
    BOOST_TEST(graph.getVertexAtIndex(4)->x == 0.6f);
    BOOST_TEST(graph.getVertexAtIndex(5)->x == 1.0f);

    BOOST_TEST(graph.getVertexCount() == 6);
}

BOOST_AUTO_TEST_CASE(graph_get_value, * boost::unit_test::tolerance((float)0.00001))
{
    spoonie::Graph graph = spoonie::Graph();
    
    graph.insertVertex(0.2f, 0.2f);
    
    BOOST_TEST(graph.getValueAt(0.0f) == 0.0f);
    BOOST_TEST(graph.getValueAt(0.5f) == 0.5f);
    BOOST_TEST(graph.getValueAt(0.1f) == 0.1f);

    graph.insertVertex(0.5f, 0.75f);
    BOOST_TEST(graph.getValueAt(0.35f) == 0.475f);
}

BOOST_AUTO_TEST_CASE(graph_set_tension)
{
    spoonie::Graph graph = spoonie::Graph();
    spoonie::Graph graphWithTension = spoonie::Graph();

    graphWithTension.setTensionAtIndex(0, 14.0f);

    BOOST_TEST(graph.getValueAt(0.35f) > graphWithTension.getValueAt(0.35f));
}

BOOST_AUTO_TEST_SUITE_END()