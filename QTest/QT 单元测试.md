近期为Qt项目调研单元测试框架，发现Gtest、BoostTest、Catch是C++单元测试框架比较常用、也是QtCreator支持调试的3种C++单元测试框架，但是这3种测试框架均不支持针对gui以及Qt信号槽的测试，这部份单元测试需要Qt自带的测试框架qTest和qtQuickTest的支持。

- **GTest** 是一个跨平台的C++单元测试框架，由google公司发布。gtest的单元测试功能完善，它提供了丰富的断言、致命和非致命判断、参数化、“死亡测试”等许多类型的宏定义，使用起来相对比较复杂。使用gtest需要依赖gtest第三方库。
  gtest还可以搭配gmock处理一些更复杂的单元测试测例，gmoke能将模块之间的接口mock起来，模拟交互过程，通过模拟数据来屏蔽其他模块对本模块单元测试模块影响。其它单元测试框架假如需要使用mock功能，也是需要搭配gmock使用。
- **Catch** ：源码就一个头文件，没有外部依赖，使用时只需要将Catch2.hpp引用到工程中即可。Catch只用一个核心的assertion宏来做测试，使用标准的C++运算符来做比较，可以分解表达式，记录表达式等号左侧和右侧的值，所以在项目中使用起来比Gtest简单。
- **BoostTest** Boost库提供的一个单元测试框架，如果项目主要使用 Boost 库，推荐使用BoostTest，可以与Boost库的log模块搭配使用。
- **QTest** ：
  1）集成在Qt内部，无需第三方库。
  2）是用于对 (Qt)UI 组件进行单元测试的唯一框架,如果要模拟测试(Qt)UI，必须使用 Qt Test。
  3）QSignalSpy类能用于验证QT的发射信号，能与其他测试框架搭配使用。
  缺点：
  1）默认不支持自动全注册，对每个测试对象需要手动QTest:：qExec();
  2）QCOMPARE无法比较不同类型的值，比较类型的宏定义比较少
  3）对Mock 支持很差
- **QtQuickTest** ：与QTest类似，主要是用于QML单元测试。

## GTest

## **一、gtest简介**

gtest是一个跨平台的(Liunx、Mac OS X、Windows 、Cygwin 、Windows CE and Symbian ) C++单元测试框架，由google公司发布。gtest是为在不同平台上为编写C++测试而生成的。它提供了丰富的断言、致命和非致命判断、参数化、“死亡测试”等等。

官方文档：[https://google.github.io/googletest/](https://link.zhihu.com/?target=https%3A//google.github.io/googletest/)

git路径：[https://github.com/google/googletest](https://link.zhihu.com/?target=https%3A//github.com/google/googletest)

## **二、gtest编译(Ubuntu)**

1. 将googletest构建为独立项目，从git获取源代码使用CMake进行构建编译生成libgtest.a和libgtest_main.a库。
   git clone [https://github.com/google/googletest.git](https://link.zhihu.com/?target=https%3A//github.com/google/googletest.git) -b release-1.11.0
   cd googletest # Main directory of the cloned repository.
   mkdir build # Create a directory to hold the build output.
   cd build
   cmake .. # Generate native build scripts for GoogleTest.
   \#or cmake .. -DBUILD_GMOCK=OFF
   make
   sudo make install # Install in /usr/local/ by default
2. 直接将 GoogleTest 构建为该项目的一部分。这是通过使 GoogleTest 源代码可用于主构建并使用 CMake 的 add_subdirectory() 命令添加它来完成的。在 GoogleTest 和项目的其余部分之间使用相同的编译器和链接器设置，因此避免了与使用不兼容的库（例如调试/发布）等相关的问题。使 GoogleTest 的源代码可用于主构建可以通过几种不同的方式完成：

- 手动下载 GoogleTest 源代码并将其放置在已知位置。这是最不灵活的方法，并且可能使其更难以与持续集成系统等一起使用。
- 将 GoogleTest 源代码作为直接副本嵌入到主项目的源代码树中。这通常是最简单的方法，但也是最难保持更新的方法。一些组织可能不允许这种方法。
- 将 GoogleTest 添加为 git 子模块或等效项。这可能并不总是可行或适当的。例如，Git 子模块有自己的优点和缺点。
- 使用 CMake 下载 GoogleTest 作为构建配置步骤的一部分。这种方法没有其他方法的限制。

下面是最后一种方法实现方式，通过CMake实现，需要3.14+ CMake版本，在项目CMakeLists.txt添加：
include(FetchContent)
FetchContent_Declare(
googletest
\# Specify the commit you depend on and update it regularly.
URL [https://github.com/google/googletest/archive/e2239ee6043f73722e7aa812a459f54a28552929.zip](https://link.zhihu.com/?target=https%3A//github.com/google/googletest/archive/e2239ee6043f73722e7aa812a459f54a28552929.zip)
)
\# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
​
\# Now simply link against gtest or gtest_main as needed. Eg
add_executable(example example.cpp)
target_link_libraries(example gtest_main)
add_test(NAME example_test COMMAND example)

## **三、gtest使用**

### 1、Assertions断言

gtest中断言是类似于函数调用的宏，可以分为两类：

- ASSERT_系列宏: 如果当前点检测失败则退出当前函数，检测点后面的代码不会再执行
- EXPECT_系列宏:如果当前点检测失败则继续往下执行

当断言失败时，googletest 会打印断言的源文件和行号位置以及失败消息。还可以operator<<提供自定义失败消息。

```cpp
ASSERT_EQ(x.size(), y.size()) << "Vectors x and y are of unequal length";

for (int i = 0; i < x.size(); ++i) {
  EXPECT_EQ(x[i], y[i]) << "Vectors x and y differ at index " << i;
}
```

### **1.1 值检查**

```cpp
后缀  参数  通过条件
//布尔条件
TRUE    (c)     c == true
FALSE   (c)     c == false
//二进制比较 (也可用于string 和 wstring)
EQ  (a, b)  a == b
NE  (a, b)  a != b
LT  (a, b)  a < b
LE  (a, b)  a <= b
GT  (a, b)  a > b
GE  (a, b)  a >= b
 //浮点数
FLOAT_EQ    (a, b)  float型 a ≈ b,彼此相差 4 个 ULP 
DOUBLE_EQ   (a, b)  double型 a ≈ b,彼此相差 4 个 ULP 
NEAR    (a, b, e)   abs(a - b) <= e
//HRESULT 成功或失败
HRESULT_SUCCEEDED   (h)     SUCCEEDED(h) == true
HRESULT_FAILED  (h)     FAILED(h) == true
//C字符串 
STREQ   (a, b)  C字符串相等
STRNE   (a, b)  C字符串不相等
STRCASEEQ   (a, b)  C字符串忽略大小写相等
STRCASENE   (a, b)  C字符串忽略大小写不相等
```

### **1.2 Predicate Assertions 谓词断言**

**1型检查函数**

1型检查函数要求返回值能隐式转为bool.

```cpp
后缀  参数  通过条件
PRED1   (f, v)  bool(f(v)) == true
PRED2   (f, v, w)   bool(f(v, w)) == true
PRED3   (f, v, w, x)    bool(f(v, w, x)) == true
PRED4   (f, v, w, x, y) bool(f(v, w, x, y)) == true
PRED5   (f, v, w, x, y, z)  bool(f(v, w, x, y, z)) == true
    
 //注意，如果给定的谓词是重载函数或函数模板，则断言宏可能无法确定要使用的版本，并且可能需要显式指定函数的类型。
//例如，对于重载的布尔函数 IsPositive() 以采用单个 int 或 double 参数，有必要编写以下内容之一：
EXPECT_PRED1(static_cast<bool (*)(int)>(IsPositive), 5);
EXPECT_PRED1(static_cast<bool (*)(double)>(IsPositive), 3.14);
//同样，要使用模板函数，请指定模板参数：
template <typename T>
bool IsNegative(T x) {
  return x < 0;
}
EXPECT_PRED1(IsNegative<int>, -5);  // Must specify type for IsNegative
//如果模板有多个参数，请将谓词括在括号中，以便正确解析宏参数：
ASSERT_PRED2((MyPredicate<int, int>), 5, 0);

```

**2型检查函数**

2型检查函数要求返回 `::testing::AssertResult`

- 定义返回值为 `::testing::AssertResult` 的检查函数.
- 在检查通过的地方返回 `::testing::AssertionSuccess()`.
- 在检查不通过的地方返回 `::testing::AssertionFailure()`, 返回前可对其使用 << 输出注释.
- 现可使用 `EXPECT_TURE(f(v))` 和 `ASSERT_TRUE(f(v))` 做自定义检查.

```cpp
::testing::AssertionResult IsEven(int n) {
    if((n % 2) == 0) {
        return ::testing::AssertionSuccess();
    } else {
        return ::testing::AssertFailure() << n << "为奇数";
    }
}
EXPECT_TRUE(IsEven(4 + 3)
```

**3型检查函数**

3型检查函数要求返回`::testing::AssertResult`, 而且要接收2n个参数, 其中前n个为C字符串(`const char*`).

- 定义返回值为 ::testing::AssertResult 的检查函数. 函数的每个接收值的形参都在函数前面添加一个接收生成值的表达式的字符串(const char*)的形参.
- 在检查通过的地方返回 ::testing::AssertionSuccess().
- 在检查不通过的地方返回 ::testing::AssertionFailure(), 返回前可对其使用 << 输出注释.
- 现可该检查函数f使用 ASSERT_PRED_FORMATn(f, v...) 和 EXPECT_PRED_FORMATn(f, v...) 做自定义检查.

```cpp
后缀  参数  通过条件
PRED_FORMAT1    (f, v)  自定义
PRED_FORMAT2    (f, v, w)   自定义
PRED_FORMAT3    (f, v, w, x)    自定义
PRED_FORMAT4    (f, v, w, x, y) 自定义
PRED_FORMAT5    (f, v, w, x, y, z)  自定义
    
    ::testing::AssertionResult f(
    const char* expr1,
    const char* expr2,
    ...
    const char* exprn,
    T1 val1,
    T2 val2,
    ...
    Tn valn);
//其中 val1, val2, ..., valn 是谓词参数的值，而 expr1, expr2, ..., exprn 是源代码中出现的相应表达式。 T1, T2, ..., Tn 类型可以是值类型也可以是引用类型；如果参数的类型为 T，则可以将其声明为 T 或 const T&，以适当者为准。 


//例如:
// Returns the smallest prime common divisor of m and n,
// or 1 when m and n are mutually prime.
int SmallestPrimeCommonDivisor(int m, int n) { ... }

// Returns true if m and n have no common divisors except 1.
bool MutuallyPrime(int m, int n) { ... }

// A predicate-formatter for asserting that two integers are mutually prime.
testing::AssertionResult AssertMutuallyPrime(const char* m_expr,
                                             const char* n_expr,
                                             int m,
                                             int n) {
  if (MutuallyPrime(m, n)) return testing::AssertionSuccess();

  return testing::AssertionFailure() << m_expr << " and " << n_expr
      << " (" << m << " and " << n << ") are not mutually prime, "
      << "as they have a common divisor " << SmallestPrimeCommonDivisor(m, n);
}

...
const int a = 3;
const int b = 4;
const int c = 10;
...
EXPECT_PRED_FORMAT2(AssertMutuallyPrime, a, b);  // Succeeds
EXPECT_PRED_FORMAT2(AssertMutuallyPrime, b, c);  // Fails

//上面的例子中，最后的断言失败并且 predicate-formatter 产生下面的失败信息：b and c (4 and 10) are not mutually prime, as they have a common divisor 2
```

### **1.3 程序流检查**

**普通**

本节中的断言直接生成成功或失败，而不是测试值或表达式。当控制流而不是布尔表达式确定测试的成功或失败时，这些很有用。

| 名称           | 参数         | 通过条件                                           |
| -------------- | ------------ | -------------------------------------------------- |
| SUCCESS        | ()           | 绝对通过                                           |
| FAIL           | ()           | 绝对不通过并中断测试(只能在返回 void 的函数中使用) |
| ADD_FAILURE    | ()           | 绝对不通过但不中断测试                             |
| ADD_FAILURE_AT | (file, line) | 在指定的文件和行号处生成非致命故障                 |

```cpp
switch(expression) {
  case 1:
    ... some checks ...
  case 2:
    ... some other checks ...
  default:
    FAIL() << "We shouldn't get here.";
}
```

**异常**

断言验证一段代码是否引发或不引发异常。使用需要在构建环境中启用异常。

| 后缀      | 参数     | 通过条件      |
| --------- | -------- | ------------- |
| THROW     | ({s}, e) | s抛出e型异常  |
| ANY_THROW | ({s})    | s抛出任意异常 |
| NO_THROW  | ({s})    | s不抛出异常   |

```cpp
//被测代码可以是复合语句
EXPECT_NO_THROW({
  int n = 5;
  DoSomething(&n);
});
```

**检查**

注意:该检查只检查本线程的检查失败, 检查不到s中创建出来的其他线程中的失败.

| 后缀             | 参数  | 通过条件        |
| ---------------- | ----- | --------------- |
| NO_FATAL_FAILURE | ({s}) | s不发生检查失败 |

**崩溃**

崩溃即程序内部无法阻止的直接的进程崩溃.

gtest将通过额外建立子进程进行崩溃测试。

::testing::FLAGS_gtest_death_test_style可设定崩溃测试的方式, 有

- threadsafe:子进程将重新执行单元测试二进制文件，就像它最初被调用的那样，但有一些额外的标志会导致只运行正在考虑的单个死亡测试
- fast:则立即执行死亡测试语句
  如果死亡测试语句运行完成而没有死亡，子进程仍然会终止，并且断言失败。

| 后缀               | 参数        | 通过条件                                                     |
| ------------------ | ----------- | ------------------------------------------------------------ |
| DEATH              | ({s}, m)    | s中以退出码0崩溃且 stderr输出匹配m                           |
| DEATH_IF_SUPPORTED | ({s}, m)    | 支持崩溃测试时, s中以退出码0崩溃且 stderr输出匹配m 不支持崩溃测试时 只编译不运行 |
| DEBUG_DEATH        | ({s}, m)    | DEBUG下,s中以退出码0崩溃且stderr输出匹配m 其他编译模式下,只运行s并恒通过 |
| EXIT               | ({s}, p, m) | s中以退出方式p崩溃且stderr输出匹配m                          |

参数 m 要么是 const std::string& 的匹配器，要么是正则表达式）

裸字符串 m（没有匹配器）被视为 ContainsRegex(m)，而不是 Eq(m)。

p可设定为以下值, 以设定退出方式.

- ::testing::ExitedWithCode(c), 退出码为c
- ::testing::KilledBySignal(s), 退出信号为s

建议崩溃测试的类名/分类名以DeathTest为后缀.

### **1.4 匹配器**

gmock库提供一些匹配器，也可以自定义匹配器，扩展gtest进行一些更复杂的单元测试。

```cpp
ASSERT_THAT(value,matcher)
EXPECT_THAT(value,matcher)

#include "gmock/gmock.h"

using ::testing::AllOf;
using ::testing::Gt;
using ::testing::Lt;
using ::testing::MatchesRegex;
using ::testing::StartsWith;

...
EXPECT_THAT(value1, StartsWith("Hello"));
EXPECT_THAT(value2, MatchesRegex("Line \\d+"));
ASSERT_THAT(value3, AllOf(Gt(5), Lt(10)));
```

### 2、Simple Tests 简单测试

TEST()宏的作用是创建一个简单测试，它定义了一个测试函数，在这个函数里可以使用任何C++代码并使用提供的断言来进行检查。如果测试中的任何断言失败（致命或非致命），或者如果测试崩溃，则整个测试失败。否则，它会成功。

```cpp
TEST(TestSuiteName, TestName) {
  ... test body ...
}
//第一个参数TestSuiteName是测试套件的名称
//第二个参数TestName是测试套件中的测试名称
//这两个名称都必须是有效的 C++ 标识符，并且它们不应包含任何下划线 (_)

//例子
int Factorial(int n);  // Returns the factorial of n
// Tests factorial of 0.
TEST(FactorialTest, HandlesZeroInput) {
  EXPECT_EQ(Factorial(0), 1);
}
// Tests factorial of positive numbers.
TEST(FactorialTest, HandlesPositiveInput) {
  EXPECT_EQ(Factorial(1), 1);
  EXPECT_EQ(Factorial(2), 2);
  EXPECT_EQ(Factorial(3), 6);
  EXPECT_EQ(Factorial(8), 40320);
}
```

### 3、Test Fixtures 测试夹具

Test Fixtures适用于多个测试场景需要相同数据配置的情况。

使用步骤：

1. 从 `::testing::Test` 派生一个自定义测试类，在自定义测试类中，声明计划使用的任何对象
2. 编写默认构造函数或 `SetUp()` 函数来为每个测试准备对象
3. 编写析构函数或 `TearDown()` 函数来释放在 构造函数/`SetUp()` 中分配的任何资源
4. 定义测试时用`TEST_F`取代`TEST`

```cpp
TEST_F(TestFixtureName, TestName) {
  ... test body ...
}
//与 TEST() 一样，第一个参数是测试套件名称，但对于 TEST_F()，这必须是自定义测试类的名称。
//每个TEST_F()运行步骤
//1、构造了一个 TestFixtureName 对象
//2、SetUp() 初始化
//3、运行TEST_F()中测试代码
//4、 测试完成调用TearDown()
//5、析构TestFixtureName 对象
```

### 4、编写 main() 函数

```cpp
int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
//::testing::InitGoogleTest() 函数解析 googletest 标志的命令行
// 不能忽略 RUN_ALL_TESTS() 的返回值，否则你会得到一个编译器告警

/**  
运行参数:(1)、系统环境变量；(2)、命令行参数；(3)、代码中指定FLAG。
命令行参数：
(1)、--gtest_list_tests:使用这个参数时，将不会执行里面的测试案例，而是输出一个案例的列表；
(2)、 --gtest_filter:对执行的测试案例进行过滤，支持通配符；
(3)、--gtest_also_run_disabled_tests:执行案例时，同时也执行被置为无效的测试案例；
(4)、--gtest_repeat=[COUNT]:设置案例重复运行次数；
(5)、--gtest_color=(yes|no|auto):输出命令行时是否使用一些五颜六色的颜色，默认是auto；
(6)、--gtest_print_time:输出命令时是否打印每个测试案例的执行时间，默认是不打印的；
(7)、--gtest_output=xml[:DIRECTORY_PATH\|:FILE_PATH:将测试结果输出到一个xml中，如—gtest_output=xml:d:\foo.xml  指定输出到d:\foo.xml ,如果不是指定了特定的文件路径，gtest每次输出的报告不会覆盖，而会以数字后缀的方式创建；
(8)、--gtest_break_on_failure:调试模式下，当案例失败时停止，方便调试；
(9)、--gtest_throw_on_failure:当案例失败时以C++异常的方式抛出；
(10)、--gtest_catch_exceptions:是否捕捉异常，gtest默认是不捕捉异常的，这个参数只在Windows下有效。
**/
```

### 5、gtest事件机制

### **5.1 TestCase事件**

继承`::testing::Test`类并实现的是`SetUp()`方法和`TearDown()`方法。

`SetUp()`方法在每个TestCase之前执行

`TearDown()`方法在每个TestCase之后执行。

### **5.2 TestSuite事件**

实现同一个类内的测试间可以通过静态变量共享对象, 以避免被测对象的反复构造析构.

- 自定义测试类public继承`::testing::Test`
- 定义并实现 `static void SetUpTestSuite()`
- 定义并实现 `static void TearDownTestSuite()`
- 定义测试时用`TEST_F`

静态对象用 SetUpTestSuite 和 TearDownTestSuite 进行创建和销毁.

gtest将在所有测试开始前调用 SetUpTestSuite, 所有测试结束后调用 TearDownTestSuite.

注意：

只定义静态对象指针, 不要定义静态对象. 否则跨文件间的静态对象的初始化顺序不可控. 虽然同一个测试类的测试间不会并行进行, 但进行顺序不是固定的. 测试中不要修改共享对象. 若做更改也必须要在测试结束时撤销修改.

### **5.3 全局事件**

可以给所有测试定义共享对象.

- 自定义类public继承 `::testing::Environment`
- 定义并实现 `void SetUp()`
- 定义并实现 `void TearDown()`
- 在 `RUN_ALL_TESTS()`前用new为本类生成一个实例, 然后用 `::testing::AddGlobalTestEnvironment` 注册为全局共享对象

gtest将接管实例并在测试开始前依次调用 SetUp 并在测试结束时反序调用 TearDown 和 delete。因此不要再delete。

注意：如果使用gtest_main，需要在`main()`之前调用它启动使其生效。

例如，可以定义一个全局变量：

```
const foo_env = AddGlobalTestEnvironment(new FooEnvironment);
```

最好还是编写自己的`main()`和调用`AddGlobalTestEnvironment()`，依赖于初始化的全局变量会使代码更难读，并可能导致当注册多个不同的环境翻译单元和环境之间有依赖关系(编译器不保证顺序初始化来自不同翻译单元的全局变量)。

### 6、值参数化测试

要写值参数化测试，首先应该定义一个fixture类。 它必须继承:: testing :: Test和:: testing :: WithParamInterface <T>（后者是纯粹的接口），其中T是参数值的类型。为了方便，可以从:: testing :: TestWithParam <T>派生fixture类，它本身是从:: testing :: Test和:: testing :: WithParamInterface <T>派生的。 T可以是任何可复制类型。如果T是一个指针，用户需要自己管理指针。使用`TEST_P`来定义具体测试。在值参测试的情况下，如果你的fixture类需要定义`SetUpTestSuite()` 或者 `TearDownTestSuite()`，他们必须是`public`的。

```cpp
class FooTest :
    public testing::TestWithParam<const char*> {
  // You can implement all the usual fixture class members here.
  // To access the test parameter, call GetParam() from class
  // TestWithParam<T>.
};
TEST_P(FooTest, DoesBlah) {
   //在测试中，使用TestWithParam <T>类的GetParam（）方法访问测试参数：
 EXPECT_TRUE(foo.Blah(GetParam()));
  ...
}

TEST_P(FooTest, HasBlahBlah) {
  ...
}
// The first parameter is just a prefix to the test suite name. 
// Pick different prefix for different instantiatioin.
// Following command will generate these test names:
// InstantiationName/FooTest.DoesBlah/0 for "meeny"
// InstantiationName/FooTest.DoesBlah/1 for "miny"
// InstantiationName/FooTest.DoesBlah/2 for "moe"
// InstantiationName/FooTest.HasBlahBlah/0 for "meeny"
// InstantiationName/FooTest.HasBlahBlah/1 for "miny"
// InstantiationName/FooTest.HasBlahBlah/2 for "moe"    
INSTANTIATE_TEST_SUITE_P(InstantiationName,
                         FooTest,
                         testing::Values("meeny", "miny", "moe"));
```

最后，需要调用`INSTANTIATE_TEST_SUITE_P`来实例化测试夹具以及指定参数集合。`gtest`定义了一系列函数方便我们来产生测试参数，相当于parameter generator。

| PARAMETER GENERATOR                         | BEHAVIOR                                                     |
| ------------------------------------------- | ------------------------------------------------------------ |
| Range(begin, end [, step])                  | Yields values {begin, begin+step, begin+step+step, ...}. The values do not include end. step defaults to 1. |
| Values(v1, v2, ..., vN)                     | Yields values {v1, v2, ..., vN}.                             |
| ValuesIn(container) and ValuesIn(begin,end) | Yields values from a C-style array, an STL-style container, or an iterator range [begin, end) |
| Bool()                                      | Yields sequence {false, true}.                               |
| Combine(g1, g2, ..., gN)                    | Yields all combinations (Cartesian product) as std::tuples of the values generated by the N generators. |

`INSTANTIATE_TEST_SUITE_P`的最后一个参数是打印参数的`functor`，作为测试名的后缀。如果不提供，会使用C++的标准输出流的重载；如果没有重载，直接打印二进制。这个函数接受一个参数`testing::TestParamInfo<class ParamType>`,返回`std::string`。

`testing::PrintToStringParamName` 是一个内置的后缀生成器， 它的返回值是`testing::PrintToString(GetParam())`。

```cpp
class MyTestSuite : public testing::TestWithParam<int> {};

TEST_P(MyTestSuite, MyTest)
{
  std::cout << "Example Test Param: " << GetParam() << std::endl;
}

INSTANTIATE_TEST_SUITE_P(MyGroup, MyTestSuite, testing::Range(0, 10),
                         testing::PrintToStringParamName());
                          
//我们也可以提供自定义的functor：
enum class MyType { MY_FOO = 0, MY_BAR = 1 };
class MyTestSuite : public testing::TestWithParam<std::tuple<MyType, std::string>> {
};

INSTANTIATE_TEST_SUITE_P(
    MyGroup, MyTestSuite,
    testing::Combine(
        testing::Values(MyType::VALUE_0, MyType::VALUE_1),
        testing::ValuesIn("", "")),
    [](const testing::TestParamInfo<MyTestSuite::ParamType>& info) {
      std::string name = absl::StrCat(
          std::get<0>(info.param) == MY_FOO ? "Foo" : "Bar", "_",
          std::get<1>(info.param));
      absl::c_replace_if(name, [](char c) { return !std::isalnum(c); }, '_');
      return name;
    });
```

## QTest

Qt Test 是一个用于对基于 Qt 的应用程序和库进行单元测试的框架。Qt Test 提供了单元测试框架中常见的所有功能以及用于测试图形用户界面的扩展。

官方文档：[https://doc.qt.io/qt-5/qttest-index.html](https://link.zhihu.com/?target=https%3A//doc.qt.io/qt-5/qttest-index.html)

## **一、QMake构建单元测试**

使用qmake构建工具，只需将以下内容添加到的项目文件中：

```cpp
QT += testlib
```

## **二、qtest使用**

### 1、宏定义

|                                                        |                                                              |
| ------------------------------------------------------ | ------------------------------------------------------------ |
| QBENCHMARK                                             | 用于测量测试中代码的性能                                     |
| QBENCHMARK_ONCE                                        | 通过运行一次来测量代码块的性能                               |
| QCOMPARE(actual, expected)                             | 使用相等运算符将实际值与预期值进行比较。如果实际和预期匹配，则继续执行。如果不是，则在测试日志中记录失败，并且测试函数返回而不尝试任何后续检查。 |
| QEXPECT_FAIL(dataIndex, comment, mode)                 | 将下一个 QCOMPARE() 或 QVERIFY() 标记为预期失败。将报告预期的失败，而不是向测试日志添加失败。如果 QVERIFY() 或 QCOMPARE() 被标记为预期失败，但通过了，则会将意外通过 (XPASS) 写入测试日志。 |
| QFAIL(message)                                         | 此宏可用于强制测试失败。测试停止执行，失败消息将附加到测试日志中。 |
| QFETCH(type, name)                                     | 在堆栈上创建一个名为 name 的局部变量，其类型为 type。名称和类型必须与测试数据表中的列匹配。这是断言，如果断言失败，测试将中止。 |
| QFETCH_GLOBAL(type, name)                              | 从全局数据表中的一行中获取一个名为 name 的变量，其类型为 type。名称和类型必须与全局数据表中的列匹配。这是断言，如果断言失败，测试将中止。 |
| QFINDTESTDATA(filename)                                | 为文件名引用的 testdata 文件返回一个 QString，如果找不到 testdata 文件，则返回一个空的 QString。 |
| QSKIP(description)                                     | 如果从测试函数调用，QSKIP() 宏会停止执行测试，而不会将失败添加到测试日志。 |
| QTEST(actual, testElement)                             | TEST() 是 QCOMPARE() 的一个便利宏，它将实际值与测试数据中的元素 testElement 进行比较。 |
| QTEST_APPLESS_MAIN(TestClass)                          | 实现一个执行 TestClass 中所有测试的 main() 函数。            |
| QTEST_GUILESS_MAIN(TestClass)                          | 实现一个 main() 函数，该函数实例化一个 QCoreApplication 对象和 TestClass，并按照定义的顺序执行所有测试。使用此宏构建独立的可执行文件。 |
| QTEST_MAIN(TestClass)                                  | 实现一个 main() 函数，该函数实例化一个应用程序对象和 TestClass，并按照定义的顺序执行所有测试。使用此宏构建独立的可执行文件。 |
| QTRY_COMPARE(actual, expected)                         |                                                              |
| QTRY_COMPARE_WITH_TIMEOUT(actual, expected, timeout)   |                                                              |
| QTRY_VERIFY2(condition, message)                       |                                                              |
| QTRY_VERIFY(condition)                                 |                                                              |
| QTRY_VERIFY2_WITH_TIMEOUT(condition, message, timeout) |                                                              |
| QTRY_VERIFY_WITH_TIMEOUT(condition, timeout)           |                                                              |
| QVERIFY2(condition, message)                           | QVERIFY2() 宏的行为与 QVERIFY() 完全相同，只是它在条件为假时报告一条消息。该消息是一个纯 C 字符串。 |
|                                                        |                                                              |
| QVERIFY(condition)                                     | QVERIFY() 宏检查条件是否为真。如果为真，则继续执行。如果不是，则在测试日志中记录失败，并且不会进一步执行测试。 |
| QVERIFY_EXCEPTION_THROWN(expression, exceptiontype)    | QVERIFY_EXCEPTION_THROWN 宏执行表达式并尝试捕获表达式抛出的异常。 |
| QWARN(message)                                         | 将消息作为警告附加到测试日志。这个宏可以在你的测试中的任何地方使用。 |

### 2、创建测试

要创建测试，请将QObject子类化并为其添加一个或多个私有槽。每个私有插槽都是您测试中的一个测试函数。QTest::qExec () 可用于执行测试对象中的所有测试函数。

```cpp
class TestQString: public QObject
{
    Q_OBJECT
private slots:
    void toUpper();
};
```

此外，可以定义以下不被视为测试函数的私有槽。如果存在，它们将由测试框架执行，可用于初始化和清理整个测试或当前测试功能。

```cpp
initTestCase();//将在执行第一个测试函数之前调用。
initTestCase_data();//将被调用以创建全局测试数据表。
cleanupTestCase();//将在执行最后一个测试函数后调用。
init();//将在每个测试函数执行之前调用。
cleanup();//将在每个测试函数之后调用。
```

用于initTestCase()准备测试。每个测试都应使系统处于可用状态，以便可以重复运行。清理操作应该在中处理cleanupTestCase()，因此即使测试失败，它们也会运行。

用于init()准备测试功能。每个测试功能都应该使系统处于可用状态，以便可以重复运行。清理操作应该在中处理cleanup()，因此即使测试功能失败并提前退出，它们也会运行。

或者，可以使用 RAII（资源获取是初始化），在析构函数中调用清理操作，以确保它们在测试函数返回并且对象移出范围时发生。

如果initTestCase()失败，则不会执行任何测试功能。如果init()失败，后面的测试函数将不被执行，测试将进入下一个测试函数。

例子：

```cpp
class MyFirstTest: public QObject
{
    Q_OBJECT

private:
    bool myCondition()
    {
        return true;
    }

private slots:
    void initTestCase()
    {
        qDebug("Called before everything else.");
    }
 void myFirstTest()
{
    QVERIFY(true); // check that a condition is satisfied
    QCOMPARE(1, 1); // compare two values
}

void mySecondTest()
{
    QVERIFY(myCondition());
    QVERIFY(1 != 2);
}

void cleanupTestCase()
{
    qDebug("Called after myFirstTest and mySecondTest.");
};
```

最后，如果测试类有一个静态公共void initMain()方法，它会在QApplication对象被实例化之前由QTEST_MAIN宏调用。例如，这允许设置像Qt::AA_DisableHighDpiScaling这样的应用程序属性。这是在 5.14 中添加的。

### 3、GUI测试

QTestlib单元测试提供GUI操作函数，可对控件发送消息后检测执行结果，比如QTest::keyClick()，QTest::mouseClick()等等。QTestlib具有测试GUI的一些特性。QTestLib发送内部Qt事件，而不是模拟本地窗口系统事件，因此运行测试程序不会对机器产生任何副作用。

QTest::keyClicks () 模拟单击小部件上的一系列键。可选地，可以指定键盘修饰符以及每次按键单击后的测试延迟（以毫秒为单位）。类似地，可以使用QTest::keyClick ()、QTest::keyPress ()、QTest::keyRelease ()、QTest::mouseClick ()、QTest::mouseDClick ()、QTest::mouseMove () 、QTest::mousePress ()和QTest::mouseRelease ()函数模拟相关的GUI事件。

举例：

```cpp
#include <QtWidgets>
#include <QtTest/QtTest>

class TestGui: public QObject
{
    Q_OBJECT
private slots:
    void testGui();
};

void TestGui::testGui()
{
    QLineEdit lineEdit;
    QTest::keyClicks(&lineEdit, "hello world");
    QCOMPARE(lineEdit.text(), QString("hello world"));
}

//QTEST_MAIN () 宏扩展为运行所有测试函数的简单 main() 方法，由于我们的测试类的声明和实现都在一个 .cpp 文件中，我们还需要包含生成的 moc 文件以使Qt 的自省工作。
QTEST_MAIN(TestGui)
#include "testgui.moc"
```

在执行测试函数时，我们首先创建一个QLineEdit。然后我们使用QTest::keyClicks ()函数在行编辑中模拟写“hello world” 。注意：小部件也必须显示才能正确测试键盘快捷键。

### 4、数据驱动测试

### **4.1数据驱动简介**

目前为止，采用硬编码的方式将测试数据写到测试函数中。如果增加更多的测试数据，那么测试函数会变成:

```cpp
QCOMPARE(QString("hello").toUpper(), QString("HELLO"));
QCOMPARE(QString("Hello").toUpper(), QString("HELLO"));
QCOMPARE(QString("HellO").toUpper(), QString("HELLO"));
QCOMPARE(QString("HELLO").toUpper(), QString("HELLO"));
```

为了不使测试函数被重复的代码弄得凌乱不堪， QTestLib支持向测试函数增加测试数据，仅需要向测试类增加另一个私有槽:

```cpp
class TestQString: public QObject
 {
     Q_OBJECT
 private slots:
     void toUpper_data();
     void toUpper();
 }; 
```

### **4.2编写测试数据函数**

为测试函数提供数据的函数必须与测试函数同名，并加上_data后缀。为测试函数提供数据的函数类似这样:

```cpp
void TestQString::toUpper_data()
 {
     QTest::addColumn<QString>("string");
     QTest::addColumn<QString>("result");
     QTest::newRow("all lower") << "hello" << "HELLO";
     QTest::newRow("mixed")     << "Hello" << "HELLO";
     QTest::newRow("all upper") << "HELLO" << "HELLO";
 }
```

首先，使用QTest::addColumn()函数定义测试数据表的两列元素：测试字符串和在该测试字符串上调用QString::toUpper()函数期望得到的结果。 然后使用 QTest::newRow()函数向测试数据表中增加一些数据。每组数据都会成为测试数据表中的一个单独的行。 QTest::newRow()函数接收一个参数：将要关联到该行测试数据的名字。如果测试函数执行失败，名字会被测试日志使用，以引用导致测试失败的数据。然后将测试数据加入到新行：首先是一个任意的字符串，然后是在该行字符串上调用 QString::toUpper()函数期望得到的结果字符串。 可以将测试数据看作是一张二维表格。在这个例子里，它包含两列三行，列名为string 和result。另外，每行都会对应一个序号和名称:

```cpp
index name string result
0 all lower “hello” HELLO
1 mixed “Hello” HELLO
2 all upper “HELLO” HELLO
```

### **4.3编写测试函数**

测试函数需要被重写：

```cpp
void TestQString::toUpper()
 {
     QFETCH(QString, string);
     QFETCH(QString, result);

     QCOMPARE(string.toUpper(), result);
 }
```

TestQString::toUpper()函数会执行两次，对toUpper_data()函数向测试数据表中加入的每一行都会调用一次。 首先，调用QFETCH()宏从测试数据表中取出两个元素。QFETCH()接收两个参数: 元素的数据类型和元素的名称。然后用QCOMPARE()宏执行测试操作。

### 5、信号测试（QSignalSpy）

[QSignalSpy](https://link.zhihu.com/?target=https%3A//doc.qt.io/qt-5/qsignalspy.html)可以连接到任何对象的任何信号并记录其发射信息。QSignalSpy本身是一个QVariant列表的列表`QList<QList<QVariant> >`。信号的每次发射都将向列表中追加一个项，其中包含信号的参数列表。

常用接口:

- 将需要记录对象和其信号传入到构造函数中。

1. `QSignalSpy(const QObject *object, PointerToMemberFunction signal)`
2. `QSignalSpy(const QObject *object, const char *signal)`

- 返回信号触发的次数。

1. `inline int count() const`

- 返回当前记录的信号名字。

1. `QByteArray QSignalSpy::signal() const`

- 获取第n次触发的信号信息。

1. `QSignalSpy::at(n)`

- 获取第n次触发的信号参数列表信息。

1. `QVariantList arguments = spy.at(n);`
2. `arguments.at(0).type();`
3. `arguments.at(1).type();`
4. `...`
5. `arguments.at(n).type();`

- 默认等待5秒超时时间，如果在超时范围内触发信号则返回true，否则返回false。

1. `bool QSignalSpy::wait(int timeout = 5000)`

例子:

```cpp
#include <QApplication>
#include <QSignalSpy>
#include <QPushButton>
#include <QDebug>

int main(int argc, char *argv[])
{
QApplication a(argc, argv);

QPushButton *btn = new QPushButton;

QSignalSpy spy(btn, &QPushButton::clicked);

  emit btn->click();

  emit btn->click();

emit btn->click();

QVariantList arguments = spy.first();

   qDebug() << "name: " << spy.signal();

 qDebug() << "type: " << arguments.at(0).type();

 qDebug() << "count: " << spy.count();

   return a.exec();

}
```

打印输出：

```text
name:  "clicked(bool)"
type:  QVariant::bool
count:  3
```

### 7、事件列表（QTestEventList）

[QTestEventList](https://link.zhihu.com/?target=https%3A//doc.qt.io/qt-5/qtesteventlist.html)类提供GUI事件的列表。可以使用GUI事件填充QTestEventList，这些GUI事件可以存储为测试数据以供以后使用，也可以在任何QWidget上重播。QTestEventList提供了用于填充列表的便捷功能。

```cpp
QTestEventList events;
events.addKeyClick('a');
events.addKeyClick(Qt::Key_Backspace);
events.addDelay(200);
QLineEdit *lineEdit = new QLineEdit(myParent);
// ...
events.simulate(lineEdit);
events.simulate(lineEdit);
```

上面的示例模拟了用户输入字符 `a` 后跟一个退格键，等待200毫秒并重复该操作。

```cpp
class AssertNoQtLogWarnings
{
    static void messageHandlerTest(QtMsgType type, const QMessageLogContext& context, const QString& msg)
    {
        static bool NO_WARNING_MSG = true;
        QByteArray localMsg = msg.toLocal8Bit();
        const char* file = context.file ? context.file : "";
        const char* function = context.function ? context.function : "";
        switch (type) {
        case QtDebugMsg:
            fprintf(stderr, "Debug: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
            break;
        case QtInfoMsg:
            fprintf(stderr, "Info: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
            break;
        case QtWarningMsg:
            EXPECT_EQ(false, NO_WARNING_MSG) << msg.toStdString();
            break;
        case QtCriticalMsg:
            EXPECT_EQ(false, NO_WARNING_MSG) << msg.toStdString();
            break;
        case QtFatalMsg:
            EXPECT_EQ(false, NO_WARNING_MSG) << msg.toStdString();
            break;
        }
    }
public:
    AssertNoQtLogWarnings()
    {
        qInstallMessageHandler(messageHandlerTest);
    }

    ~AssertNoQtLogWarnings()
    {
        //Install default message handler
        qInstallMessageHandler(nullptr);
    }
};

#include <QAbstractItemModelTester>
TEST(QAbstractItemModel, QAbstractItemModel)
{
    //RAII object. Any warning, critical or fatal message produced in this context
    //will produce a GTest fail assertion
    AssertNoQtLogWarnings logQtTest;

    MyAbstractItemModel model;
    QAbstractItemModelTester tester(&model, QAbstractItemModelTester::FailureReportingMode::Warning);

}
```

## QtQuickTest

[Qt Quick Test](https://link.zhihu.com/?target=https%3A//doc.qt.io/qt-5/qtquicktest-index.html) 是 QML 应用程序的单元测试框架。使用QtQuickTest只需要在pro文件中添加 `CONFIG += qmltestcase`即可。

测试用例被编写为 TestCase 类型中的 JavaScript 函数，名称以 test 开头的函数被视为要执行的测试用例。QUICK_TEST_MAIN(example)宏会启用指定源目录（QUICK_TEST_SOURCE_DIR定义）及子目录下所有“tst**.qml”文件中所有测试用例，如果未定义 QUICK_TEST_SOURCE_DIR，将扫描当前目录及子目录下所有“tst**.qml”文件。

```cpp
//cpp
#include <QtQuickTest>
QUICK_TEST_MAIN(example)
```

可以使用 `QUICK_TEST_MAIN_WITH_SETUP` 宏 在QML 测试之前执行 C++代码，这对于在 QML 引擎上设置上下文属性等非常有用。它需要一个额外的 QObject* 参数。测试框架将调用具有以下名称的槽和可调用函数：

| NAME                                  | PURPOSE                                                      |
| ------------------------------------- | ------------------------------------------------------------ |
| void applicationAvailable()           | 在实例化 QApplication 对象后立即调用。使用此函数执行不需要 QQmlEngine 实例的设置。 |
| void qmlEngineAvailable(QQmlEngine *) | 当 QQmlEngine可用时调用。此时，引擎上将设置任何导入路径、插件路径和额外的文件选择器。每个 QML 测试文件都会调用一次此函数，因此任何参数对于该测试都是唯一的。例如，这意味着每个 QML 测试文件都有自己的 QQmlEngine。此功能可用于注册 QML 类型和添加导入路径等。 |
| void cleanupTestCase()                | 在测试执行完成后立即调用。使用此功能在一切开始被破坏之前进行清理。 |

```cpp
//使用宏在 QML 引擎上设置上下文属性
// tst_mytest.cpp
#include <QtQuickTest>
#include <QQmlEngine>
#include <QQmlContext>

class Setup : public QObject
{
    Q_OBJECT

public:
    Setup() {}

public slots:
    void qmlEngineAvailable(QQmlEngine *engine)
    {
        engine->rootContext()->setContextProperty("myContextProperty", QVariant(true));
    }
};

QUICK_TEST_MAIN_WITH_SETUP(mytest, Setup)

#include "tst_mytest.moc"
```

## **SignalSpy**

和QSignalSpy类似。用于观察QML类型上的信号。

- ***\****[count](https://link.zhihu.com/?target=https%3A//doc.qt.io/qt-5/qml-qttest-signalspy.html%23count-prop)***\**** : int //自上次调用 clear() 以来从目标发出 signalName 的次数
- ***\****[signalArguments](https://link.zhihu.com/?target=https%3A//doc.qt.io/qt-5/qml-qttest-signalspy.html%23signalArguments-prop)***\**** : list //包含发出的信号参数的列表。信号的每次发射都会将一项添加到列表中，其中包含信号的参数。当连接到新目标或新信号名或调用 clear() 方法时，signalArguments 将被重置为空
- ***\****[signalName](https://link.zhihu.com/?target=https%3A//doc.qt.io/qt-5/qml-qttest-signalspy.html%23signalName-prop)***\**** : string //定义目标上要侦听的信号的名称
- ***\****[target](https://link.zhihu.com/?target=https%3A//doc.qt.io/qt-5/qml-qttest-signalspy.html%23target-prop)***\**** : var //用于侦听 signalName 信号发射的目标对象
- ***\****[valid](https://link.zhihu.com/?target=https%3A//doc.qt.io/qt-5/qml-qttest-signalspy.html%23valid-prop)***\**** : bool //定义当前信号连接状态。目标的signalName连接成功时为真，否则为假

`clear()`:将 count 清零，将 valid 重置为 false 并将 signalArguments 清空。

`wait(*timeout* = 5000)`:等待目标上的信号 signalName 发出，最多超时毫秒。如果没有发出信号，测试用例将失败。

```js
//SignalSpy
Button {
    id: button
    SignalSpy {
        id: spy
        target: button
        signalName: "clicked"
    }
    TestCase {
        name: "ButtonClick"
        function test_click() {
            compare(spy.count, 0)
            button.clicked();
            compare(spy.count, 1)
        }
    }
}
```

## **TestCase**

QML测试用例均被写成TestCase类型中的JavaScript函数。

```js
import QtQuick 2.0
import QtTest 1.2

TestCase {
    name: "MathTests"

    function test_math() {
        compare(2 + 2, 4, "2 + 2 = 4")
    }

    function test_fail() {
        compare(2 + 2, 5, "2 + 2 = 5")
    }
}
```

名称以“ test_”开头的函数被视为要执行的测试用例。该名称属性被用于前缀在输出的功能：

```text
********* Start testing of MathTests *********
Config: Using QTest library 4.7.2, Qt 4.7.2
PASS   : MathTests::initTestCase()
FAIL!  : MathTests::test_fail() 2 + 2 = 5
   Actual (): 4
   Expected (): 5
   Loc: [/home/.../tst_math.qml(12)]
PASS   : MathTests::test_math()
PASS   : MathTests::cleanupTestCase()
Totals: 3 passed, 1 failed, 0 skipped
********* Finished testing of MathTests *********
```

由于JavaScript属性的工作方式,找到测试函数的顺序是不可预测的。为了帮助实现可预测性,测试框架将根据名称的升序对函数进行排序。当有两个测试必须按顺序运行时,这可以帮助你。

可以提供多种TestCase类型。全部完成后，测试程序将退出。如果不需要运行测试用例（因为前提条件失败），则可以将optional设置为true。

### 属性

- ***\****[completed](https://link.zhihu.com/?target=https%3A//doc.qt.io/qt-5/qml-qttest-testcase.html%23completed-prop)***\**** : bool //一旦测试用例完成执行，此属性将设置为 true。测试用例只执行一次。初始值为假。
- ***\****[name](https://link.zhihu.com/?target=https%3A//doc.qt.io/qt-5/qml-qttest-testcase.html%23name-prop)***\**** : string //定义结果报告的测试用例的名称。默认值为空字符串
- ***\****[optional](https://link.zhihu.com/?target=https%3A//doc.qt.io/qt-5/qml-qttest-testcase.html%23optional-prop)***\**** : bool //可以在测试应用程序中提供多种 TestCase 类型。全部完成后，应用程序将退出。如果不需要运行测试用例（因为前提条件失败），则可以将此属性设置为 true。默认值为假。
- ***\****[running](https://link.zhihu.com/?target=https%3A//doc.qt.io/qt-5/qml-qttest-testcase.html%23running-prop)***\**** : bool //在测试用例运行时，此属性将设置为 true。初始值为假，一旦测试用例完成，该值将再次变为假。
- ***\****[when](https://link.zhihu.com/?target=https%3A//doc.qt.io/qt-5/qml-qttest-testcase.html%23when-prop)***\**** : bool //当应用程序希望测试用例运行时，此属性应设置为 true。默认值是true。在以下示例中，当用户按下鼠标按钮时运行测试：
- ***\****[windowShown](https://link.zhihu.com/?target=https%3A//doc.qt.io/qt-5/qml-qttest-testcase.html%23windowShown-prop)***\**** : bool //在显示 QML 查看窗口后，此属性将设置为 true。通常测试用例会在测试应用程序加载后并在显示窗口之前运行。如果测试用例涉及视觉类型和行为，那么它可能需要延迟到显示窗口之后。

### 数据驱动的测试

可以使用以“ _data”结尾的函数名称将表数据提供给测试。另外， `init_data()` 函数可用于为TestCase类型的所有测试函数提供默认测试数据：

```js
import QtQuick 2.0
import QtTest 1.2

TestCase {
    name: "DataTests"

    function init_data() {
      return [
           {tag:"init_data_1", a:1, b:2, answer: 3},
           {tag:"init_data_2", a:2, b:4, answer: 6}
      ];
    }

    function test_table_data() {
        return [
            {tag: "2 + 2 = 4", a: 2, b: 2, answer: 4 },
            {tag: "2 + 6 = 8", a: 2, b: 6, answer: 8 },
        ]
    }

    function test_table(data) {
        //数据来自test_table_data
        compare(data.a + data.b, data.answer)
    }

    function test__default_table(data) {
        //数据来自init_data
        compare(data.a + data.b, data.answer)
    }
}
```

测试框架将遍历表中的所有行，并将每一行传递给测试函数。如图所示，可以提取这些列以用于测试。该 `tag` 栏是特殊的-它是由测试框架打印时连续失败，以帮助读者识别哪些情况下未能集合之中，否则测试通过。

### Benchmarks

名称以“ benchmark_”开头的函数将在Qt基准框架中运行多次，并报告运行的平均时序值。这等效于在C ++版本的QTestLib中使用 `QBENCHMARK` 宏。

```js
TestCase {
    id: top
    name: "CreateBenchmark"

    function benchmark_create_component() {
        var component = Qt.createComponent("item.qml")
        var obj = component.createObject(top)
        obj.destroy()
        component.destroy()
    }
}

RESULT : CreateBenchmark::benchmark_create_component:
     0.23 msecs per iteration (total: 60, iterations: 256)
PASS   : CreateBenchmark::benchmark_create_component()
```

要获得 `QBENCHMARK_ONCE` 宏的效果，请在测试函数名称前添加“ benchmark_once_”。

### 模拟键盘和鼠标事件

keyPress ()、keyRelease ( ) 和keyClick () 方法可用于模拟单元测试中的键盘事件。事件被传递到当前聚焦的 QML 项目。您可以传递 Qt.Key 枚举值或 latin1 char（长度为 1 的字符串）

```js
Rectangle {
    width: 50; height: 50
    focus: true

    TestCase {
        name: "KeyClick"
        when: windowShown

        function test_key_click() {
            keyClick(Qt.Key_Left)
            keyClick("a")
            ...
        }
    }
}
```

mousePress ()、mouseRelease ()、mouseClick ()、mouseDoubleClickSequence ()和mouseMove ()方法可以类似的方式用来模拟鼠标事件。

注意：只有在显示主窗口后才能传递键盘和鼠标事件。尝试在此之前传递事件将失败。使用when和windowShown属性跟踪显示主窗口的时间。

### 管理动态创建的测试对象

QML测试的典型模式是动态创建一个项目，然后在测试功能结束时销毁它：

```text
TestCase {
    id: testCase
    name: "MyTest"
    when: windowShown

    function test_click() {
        var item = Qt.createQmlObject("import QtQuick 2.0; Item {}", testCase);
        verify(item);

        // 测试项目...

        item.destroy();
    }
}
```

这种模式的问题在于，测试功能中的任何失败都将导致对 `item.destroy()` 的调用被跳过，从而使该项目一直挂在场景中，直到测试用例完成为止。这可能会干扰以后的测试；例如，通过阻止输入事件或产生不相关的调试输出，使跟踪代码执行变得困难。

通过调用[createTemporaryQmlObject](https://link.zhihu.com/?target=https%3A//runebook.dev/zh-CN/docs/qt/qml-qttest-testcase%23createTemporaryQmlObject-method) () 代替，可以保证对象在测试函数结束时被销毁：

```text
TestCase {
    id: testCase
    name: "MyTest"
    when: windowShown

    function test_click() {
        var item = createTemporaryQmlObject("import QtQuick 2.0; Item {}", testCase);
        verify(item);

        // 测试项目...

        //不必担心在这里破坏“项目”。
    }
}
```

对于通过Component的createObject () 函数创建的对象，可以使用createTemporaryObject () 函数。

## Catch

可以从GitHub下载完整的[Catch](https://link.zhihu.com/?target=https%3A//github.com/catchorg/Catch2/blob/devel/docs/tutorial.md%23top)

## **一、使用 Catch**

Catch是header-only的，故你只需要将Catch的头文件放到编译器可以发现的路径既可。

下面的教程默认你的编译器可以发现并使用 Catch。

*如果你使用Catch的预编译形式，即已经编译并生成了Catch链接库（.lib 或者 .a 文件），你的Catch头文件包含形式应该形如：*`*#include <catch2/catch.hpp>*` *。v2版本的Catch项目路径是：*[https://github.com/catchorg/Catch2/tree/v2.x](https://link.zhihu.com/?target=https%3A//github.com/catchorg/Catch2/tree/v2.x)

## **二、编写测试用例**

让我们从一个简单的示例开始(examples/010-TestCase.cpp)。假设你已经写了一个用于计算阶乘的函数，现在准备测试它。

```cpp
unsigned int Factorial( unsigned int number ) {
    return number <= 1 ? number : Factorial(number-1)*number;
}
```

为了尽量简单，我们把所有的代码都放到一个源文件中。

```cpp
#define CATCH_CONFIG_MAIN  // 当前宏强制Catch在当前编译单元中创建 main()，这个宏只能出现在一个CPP文件中，因为一个项目只能有一个有效的main函数
#include "catch.hpp"

unsigned int Factorial( unsigned int number ) {
    return number <= 1 ? number : Factorial(number-1)*number;
}

TEST_CASE( "Factorials are computed", "[factorial]" ) {
    REQUIRE( Factorial(1) == 1 );
    REQUIRE( Factorial(2) == 2 );
    REQUIRE( Factorial(3) == 6 );
    REQUIRE( Factorial(10) == 3628800 );
}
```

编译结束后将生成一个可以接受运行时参数的可执行文件，具体可用参数请参考command-line.md。如果以不带参数的方式执行可执行文件，所有测试用例都将被执行。详细的测试报告将输出到终端，测试报告包含失败的测试用例、失败的测试用例个数、成功的测试用例个数等信息。

执行上面代码生成的可执行文件，所有测试用例都将通过。真的没有错误吗？不是的，上面的阶乘函数是有错误的。

这个错误是什么呢？0的阶乘是多少？——0的阶乘是1而不是0，这就是上面阶乘函数的错误之处。

让我们把上面的规则写入到测试用例中：

```cpp
TEST_CASE( "Factorials are computed", "[factorial]" ) {
    REQUIRE( Factorial(0) == 1 );
    REQUIRE( Factorial(1) == 1 );
    REQUIRE( Factorial(2) == 2 );
    REQUIRE( Factorial(3) == 6 );
    REQUIRE( Factorial(10) == 3628800 );
}
```

现在测试失败了，Catch输出：

```css
Example.cpp:9: FAILED:
  REQUIRE( Factorial(0) == 1 )
with expansion:
  0 == 1
```

Catch的测试报告会输出期望值和Factorial(0)计算出的错误值0，这样我们就可以很方便的找到错误。

让我们修正阶乘函数:

```cpp
unsigned int Factorial( unsigned int number ) {
  return number > 1 ? Factorial(number-1)*number : 1;
}
```

现在所有的测试用例都通过了。

当然了上面的阶乘函数依旧有不少问题，例如当number很大时计算的结果将溢出，不过我们暂不管这些。

虽然上面的测试比较简单，但已经足够展示如何使用Catch了。在进一步学习前，我们先解释一下上面那段代码。

1. 我们定义了一个宏，并包含了Catch的头文件，然后编译这个源文件并生成了一个接受运行时参数的可执行文件。为了可执行，定义了宏`#define CATCH_CONFIG_MAIN`，强制Catch引入预定义main函数，你也可以编写自己的main函数）。
2. 我们在宏`TEST_CASE`中编写测试用例。这个宏可以包含一个或者两个参数，其中一个参数是没有固定格式的测试名，另一个参数则包含一个或多个标签（下文介绍）。测试名必须唯一。参考command-line.md以获得更多有关执行可执行文件的信息。
3. 测试名和标签都是字符串。
4. 我们仅使用宏`REQUIRE`来编写测试断言。Catch没有使用分立的测试函数表示不同的断言（例如REQUIRE_TRUE、REQUIRE_FALSE、REQUIRE_EQUAL、REQUIRE_LESS等），而是直接使用C++表达式的真值结果。此外Catch使用模板表达式捕获测试表达式的左侧和右侧（例如 `exp_a == exp_b`，Catch将捕获exp_a和exp_b的表达式结果），从而在测试报告中显示两侧的计算结果。

## **三、测试用例和测试区段（Test case and section）**

大部分测试框架都有某种基于类的机制。例如，在很多框架（例如JUnit）的`setup()`阶段可以创建一个在其他用例中使用的测试对象（可以是需要测试的对象，也可以是Mock对象），在`teardown()`阶段销毁这些对象，从而避免在每一个测试用例中创建与销毁测试对象（或mock对象）。

使用上面传统的测试方式有一定的缺陷，例如对于同一批测试用例你只能创建同一个测试对象，这样的话测试粒度就比较大。（译者注：其他缺陷可以参考原文）

Catch 使用全新的方式解决了上面的问题，如下：

```cpp
TEST_CASE( "vectors can be sized and resized", "[vector]" ) {

    std::vector<int> v( 5 );

    REQUIRE( v.size() == 5 );
    REQUIRE( v.capacity() >= 5 );

    SECTION( "resizing bigger changes size and capacity" ) {
        v.resize( 10 );

        REQUIRE( v.size() == 10 );
        REQUIRE( v.capacity() >= 10 );
    }
    SECTION( "resizing smaller changes size but not capacity" ) {
        v.resize( 0 );

        REQUIRE( v.size() == 0 );
        REQUIRE( v.capacity() >= 5 );
    }
    SECTION( "reserving bigger changes capacity but not size" ) {
        v.reserve( 10 );

        REQUIRE( v.size() == 5 );
        REQUIRE( v.capacity() >= 10 );
    }
    SECTION( "reserving smaller does not change size or capacity" ) {
        v.reserve( 0 );

        REQUIRE( v.size() == 5 );
        REQUIRE( v.capacity() >= 5 );
    }
}
```

对于每一个`SECTION`，`TEST_CASE`都将重新从当前`TEST_CASE`的起始部分开始执行并忽略其他`SECTION`。 （译者注：这段原文简单解释了原因，Catch使用了if语句并把section看做子节点，每次执行TEST_CASE时Catch先执行起始部分的非`SECTION`代码，然后选择一个子节点并执行）。

到目前为止，Catch使用上述方式已经实现了大部分测试框架基于类（setup&teardown）的测试机制。

`SECTION`可以嵌套任意深度，每一个`SECTION`子节点都只会被执行一次，大量嵌套的`SECTION`会形成一棵“树”，父节点执行失败将不再执行对应的子节点。下面是嵌套使用`SECTION`的例子：

```cpp
    SECTION( "reserving bigger changes capacity but not size" ) {
        v.reserve( 10 );

        REQUIRE( v.size() == 5 );
        REQUIRE( v.capacity() >= 10 );

        SECTION( "reserving smaller again does not change capacity" ) {
            v.reserve( 7 );

            REQUIRE( v.capacity() >= 10 );
        }
    }
```

## **四、BDD-Style**

Catch可以使用BDD-Style形式的测试，具体请参考：test-cases-and-sections.md，下面是一个简单的例子：

```cpp
SCENARIO( "vectors can be sized and resized", "[vector]" ) {

    GIVEN( "A vector with some items" ) {
        std::vector<int> v( 5 );

        REQUIRE( v.size() == 5 );
        REQUIRE( v.capacity() >= 5 );

        WHEN( "the size is increased" ) {
            v.resize( 10 );

            THEN( "the size and capacity change" ) {
                REQUIRE( v.size() == 10 );
                REQUIRE( v.capacity() >= 10 );
            }
        }
        WHEN( "the size is reduced" ) {
            v.resize( 0 );

            THEN( "the size changes but not capacity" ) {
                REQUIRE( v.size() == 0 );
                REQUIRE( v.capacity() >= 5 );
            }
        }
        WHEN( "more capacity is reserved" ) {
            v.reserve( 10 );

            THEN( "the capacity changes but not the size" ) {
                REQUIRE( v.size() == 5 );
                REQUIRE( v.capacity() >= 10 );
            }
        }
        WHEN( "less capacity is reserved" ) {
            v.reserve( 0 );

            THEN( "neither size nor capacity are changed" ) {
                REQUIRE( v.size() == 5 );
                REQUIRE( v.capacity() >= 5 );
            }
        }
    }
}
```

运行上面的测试用例将输出以下内容:

```text
Scenario: vectors can be sized and resized
     Given: A vector with some items
      When: more capacity is reserved
      Then: the capacity changes but not the size
```

## **五、类型参数化测试**

Catch支持类型参数化测试，宏`TEMPLATE_TEST_CASE`和`TEMPLATE_PRODUCT_TEST_CASE`的行为和`TEST_CASE`类似，但测试用例会在不同类型下执行。下面代码中`TestType`的取值依次为`int`、`float`、`std::string`、`Bar`，所有测试用例都将在这些类型下执行一遍。

```cpp
struct Bar {}; 

TEMPLATE_TEST_CASE("Templated test","",int,float, std::string, Bar)
{
    std::vector<TestType> v( 5 );
    REQUIRE( v.size() == 5 );
    REQUIRE( v.capacity() >= 5 );
}
```

