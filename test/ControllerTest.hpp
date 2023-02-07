#ifndef ControllerTest_hpp
#define ControllerTest_hpp

#include "oatpp-test/UnitTest.hpp"

class ControllerTest : public oatpp::test::UnitTest {
public:

  ControllerTest() : UnitTest("TEST[ControllerTest]"){}
  void onRun() override;

};

#endif // ControllerTest_hpp
