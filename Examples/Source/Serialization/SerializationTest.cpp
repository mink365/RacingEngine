#include "SerializationTest.h"

#include <yaml-cpp/yaml.h>

struct Person {
    BOOST_HANA_DEFINE_STRUCT(Person,
                             (std::string, name),
                             (int, age)
                             );
};

namespace test
{

struct One
{
    One(void) :
    two(0),
    three(0.0),
    four(false)
    {}

    One(int a, double b, bool c) :
    two(a),
    three(b),
    four(c)
    {}

    int two;
    double three;

//private:
    bool four;
};

struct Five
{
    typedef std::map<int, One> Map_t;
    Map_t six;

    typedef std::vector<One> Vector_t;
    Vector_t seven;

    std::string eight;

    typedef std::vector<std::vector<double> > Matrix_t;
    Matrix_t nine;
};
}

BOOST_HANA_ADAPT_STRUCT
(
    test::One,
    two,
    three,
    four
);

BOOST_HANA_ADAPT_STRUCT
(
    test::Five,
    six,
    seven,
    eight,
    nine
);

using namespace test;
SerializationTest::SerializationTest()
{
    this->name = "SerializationTest";
}

void TestYaml()
{
    YAML::Node node = YAML::Load("foo: bar\nx: 2.23");

//    Vec3 v = node.as<Vec3>();
//    double v = node[0].as<double>();

    YAML::Node pi = node["pi"];

    re::Log("Value: {} vv: {}", node["foo"].as<std::string>(), node["x"].as<double>());
}

void SerializationTest::Init()
{
    Five v;
    v.six.insert(std::make_pair(123, One(3, 6.6, true)));
    v.six.insert(std::make_pair(456, One(4, 8.8, false)));

    v.seven.push_back(One(5, 1.1, true));
    v.seven.push_back(One(5, 2.2, true));

    v.eight = "eight";

    std::vector<double> temp;
    temp.push_back(2.3);
    temp.push_back(4.5);

    v.nine.push_back(temp);
    v.nine.push_back(temp);

    One one{10, 5.5f, true};

//    serialize(std::cout, one);

    re::Log("json string: {}", to_json(v));

    TestYaml();
}

