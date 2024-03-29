#include "dbc.hpp"
#include "gtest/gtest.h"

TEST(DBCTests, simpleInvariant) {
  DBC_INVARIANT(simpleInvariant) << []{return true;};
}

TEST(DBCTests, failingInvariantPre) {
  EXPECT_DEATH({DBC_INVARIANT(failingPre) << []{return false;};},
               "Assertion");
}

void failedPostInvariant(){
  bool x = true;
  DBC_INVARIANT(failingPost) << [&x]{return x;};
  SUCCEED();
  x = false;
}

TEST(DBCTests, failingInvariantPost) {
  EXPECT_DEATH(failedPostInvariant(), 
               "Assertion");
}

void trivialAssumeGuarantee(){
  DBC_ASSUME_GUARANTEE(trivialAG);
}

TEST(DBCTests, trivialAssumeGauarantee) {
  trivialAssumeGuarantee();
  SUCCEED();
}

void simpleAssumeGuarantee(){
  DBC_ASSUME_GUARANTEE(simpleAG)
    << dbc::assume << []{return (2+4 > 0);};
}

TEST(DBCTests, simpleAssumeGauarantee) {
  simpleAssumeGuarantee();
  SUCCEED();
}

void simpleAssumeGuaranteeFailPre(){
  DBC_ASSUME_GUARANTEE(simpleAG)
    << dbc::assume << []{return (false);};
  FAIL();
}

TEST(DBCTests, simpleAssumeGauaranteeFailPre) {
  EXPECT_DEATH(simpleAssumeGuaranteeFailPre(), 
               "Assertion");
}

void simpleAssumeGuaranteeFailPost(){
  DBC_ASSUME_GUARANTEE(simpleAG) <<
    dbc::assume << []{return true;} <<
    dbc::guarantee << []{return false;};
  SUCCEED();
}

TEST(DBCTests, simpleAssumeGauaranteeFailPost) {
  EXPECT_DEATH(simpleAssumeGuaranteeFailPost(), 
               "Assertion");
}

void trivialPrePost() {
  int x;
  DBC_PRE_POST(trivialPrePost, [&]{return x;});
}

TEST(DBCTests, trivialPrePost) {
  trivialPrePost();
  SUCCEED();
}


void simplePrePost() {
  int x = 0;

  DBC_PRE_POST(trivialPrePost, [&]{return x;}) <<
    dbc::pre << [](int pre){return(pre == 0);} <<
    dbc::post << [](int, int post){return(post != 0);} ;
  
  x = 1;
}

TEST(DBCTests, simplePrePost) {
  simplePrePost();
  SUCCEED();
}

void simplePrePostFailPre() {
  int x = 1;

  DBC_PRE_POST(trivialPrePost, [&]{return x;}) <<
    dbc::pre << [](int pre){return(pre == 0);} ;
  
  FAIL();
}

TEST(DBCTests, simplePrePostFailPre) {
  EXPECT_DEATH(simplePrePostFailPre(), 
               "Assertion");
}

void simplePrePostFailPost() {
  int x = 0;

  DBC_PRE_POST(trivialPrePost, [&]{return x;}) <<
    dbc::pre << [](int pre){return(pre == 0);} <<
    dbc::post << [](int, int post){return(post == 0);} ;

  SUCCEED();
  x ++;
}

TEST(DBCTests, simplePrePostFailPost) {
  EXPECT_DEATH(simplePrePostFailPost(), 
               "Assertion");
}

class ClassWithInvariant {
private:
  int x;
  int y;

  bool classInvariant_() {
    return y == x + 2;
  }

public:
  ClassWithInvariant(int a) :
    x(a),
    y(x+2)
  {
    DBC_CLASS_INVARIANT(construction);
  }

  ClassWithInvariant(int a, int b) :
    x(a),
    y(b)
  {
    DBC_CLASS_INVARIANT(badconstruction);
  }

  void addOne() {
    DBC_CLASS_INVARIANT(addOne);

    x++;
    y++;
  }

  void addOneBad() {
    DBC_CLASS_INVARIANT(addOne);

    y++;
  }
};

TEST(DBCTests, classInvariants) {
  ClassWithInvariant c1(3);
  SUCCEED();
  EXPECT_DEATH({ClassWithInvariant c2(3,4);}, 
               "Assertion");
  c1.addOne();
  EXPECT_DEATH({c1.addOneBad();}, 
               "Assertion");
}

void loopVariantFail() {
  int a = 0;

  while (a < 10) {
    DBC_LOOP(loopVariantFail, [&]{return a;});
  }
}

TEST(DBCTests, loopVariantFail) {
  EXPECT_DEATH(loopVariantFail(), 
               "Assertion");
}

TEST(DBCTests, loopVariantTrue) {
  int a = 10;

  while (a >= -10) {
    DBC_LOOP(loopVariantTrue, [&]{return a + 10;});

    a --;
  }
}

void loopInvariantFail() {
  int a = 10;

  while (a > 0) {
    DBC_LOOP(loopInvariantFail, [&]{return a;})
      << dbc::loop_invariant << [&](){return a > 2;};
    a --;
  }
}

TEST(DBCTests, loopInvariantFail) {
  EXPECT_DEATH(loopInvariantFail(), 
               "Assertion");
}

TEST(DBCTests, loopInvariantTrue) {
  int a = 10;

  while (a > 0) {
    DBC_LOOP(loopInvariantTrue, [&]{return a;})
      << dbc::loop_invariant << [&](){return a > 0;};
    a --;
  }
}
