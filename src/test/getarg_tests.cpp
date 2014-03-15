#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>
#include <boost/test/unit_test.hpp>

#include "util.h"

BOOST_AUTO_TEST_SUITE(getarg_tests)

static void
ResetArgs(const std::string& strArg)
{
    std::vector<std::string> vecArg;
    boost::split(vecArg, strArg, boost::is_space(), boost::token_compress_on);

    // Insert dummy executable name:
    vecArg.insert(vecArg.begin(), "testbitcoin");

    // Convert to char*:
    std::vector<const char*> vecChar;
    BOOST_FOREACH(std::string& s, vecArg)
        vecChar.push_back(s.c_str());

    ParseParameters(vecChar.size(), &vecChar[0]);
}

BOOST_AUTO_TEST_CASE(boolarg)
{
    ResetArgs("-BEC");
    BOOST_CHECK(GetBoolArg("-BEC"));
    BOOST_CHECK(GetBoolArg("-BEC", false));
    BOOST_CHECK(GetBoolArg("-BEC", true));

    BOOST_CHECK(!GetBoolArg("-fo"));
    BOOST_CHECK(!GetBoolArg("-fo", false));
    BOOST_CHECK(GetBoolArg("-fo", true));

    BOOST_CHECK(!GetBoolArg("-BECo"));
    BOOST_CHECK(!GetBoolArg("-BECo", false));
    BOOST_CHECK(GetBoolArg("-BECo", true));

    ResetArgs("-BEC=0");
    BOOST_CHECK(!GetBoolArg("-BEC"));
    BOOST_CHECK(!GetBoolArg("-BEC", false));
    BOOST_CHECK(!GetBoolArg("-BEC", true));

    ResetArgs("-BEC=1");
    BOOST_CHECK(GetBoolArg("-BEC"));
    BOOST_CHECK(GetBoolArg("-BEC", false));
    BOOST_CHECK(GetBoolArg("-BEC", true));

    // New 0.6 feature: auto-map -nosomething to !-something:
    ResetArgs("-noBEC");
    BOOST_CHECK(!GetBoolArg("-BEC"));
    BOOST_CHECK(!GetBoolArg("-BEC", false));
    BOOST_CHECK(!GetBoolArg("-BEC", true));

    ResetArgs("-noBEC=1");
    BOOST_CHECK(!GetBoolArg("-BEC"));
    BOOST_CHECK(!GetBoolArg("-BEC", false));
    BOOST_CHECK(!GetBoolArg("-BEC", true));

    ResetArgs("-BEC -noBEC");  // -BEC should win
    BOOST_CHECK(GetBoolArg("-BEC"));
    BOOST_CHECK(GetBoolArg("-BEC", false));
    BOOST_CHECK(GetBoolArg("-BEC", true));

    ResetArgs("-BEC=1 -noBEC=1");  // -BEC should win
    BOOST_CHECK(GetBoolArg("-BEC"));
    BOOST_CHECK(GetBoolArg("-BEC", false));
    BOOST_CHECK(GetBoolArg("-BEC", true));

    ResetArgs("-BEC=0 -noBEC=0");  // -BEC should win
    BOOST_CHECK(!GetBoolArg("-BEC"));
    BOOST_CHECK(!GetBoolArg("-BEC", false));
    BOOST_CHECK(!GetBoolArg("-BEC", true));

    // New 0.6 feature: treat -- same as -:
    ResetArgs("--BEC=1");
    BOOST_CHECK(GetBoolArg("-BEC"));
    BOOST_CHECK(GetBoolArg("-BEC", false));
    BOOST_CHECK(GetBoolArg("-BEC", true));

    ResetArgs("--noBEC=1");
    BOOST_CHECK(!GetBoolArg("-BEC"));
    BOOST_CHECK(!GetBoolArg("-BEC", false));
    BOOST_CHECK(!GetBoolArg("-BEC", true));

}

BOOST_AUTO_TEST_CASE(stringarg)
{
    ResetArgs("");
    BOOST_CHECK_EQUAL(GetArg("-BEC", ""), "");
    BOOST_CHECK_EQUAL(GetArg("-BEC", "eleven"), "eleven");

    ResetArgs("-BEC -bar");
    BOOST_CHECK_EQUAL(GetArg("-BEC", ""), "");
    BOOST_CHECK_EQUAL(GetArg("-BEC", "eleven"), "");

    ResetArgs("-BEC=");
    BOOST_CHECK_EQUAL(GetArg("-BEC", ""), "");
    BOOST_CHECK_EQUAL(GetArg("-BEC", "eleven"), "");

    ResetArgs("-BEC=11");
    BOOST_CHECK_EQUAL(GetArg("-BEC", ""), "11");
    BOOST_CHECK_EQUAL(GetArg("-BEC", "eleven"), "11");

    ResetArgs("-BEC=eleven");
    BOOST_CHECK_EQUAL(GetArg("-BEC", ""), "eleven");
    BOOST_CHECK_EQUAL(GetArg("-BEC", "eleven"), "eleven");

}

BOOST_AUTO_TEST_CASE(intarg)
{
    ResetArgs("");
    BOOST_CHECK_EQUAL(GetArg("-BEC", 11), 11);
    BOOST_CHECK_EQUAL(GetArg("-BEC", 0), 0);

    ResetArgs("-BEC -bar");
    BOOST_CHECK_EQUAL(GetArg("-BEC", 11), 0);
    BOOST_CHECK_EQUAL(GetArg("-bar", 11), 0);

    ResetArgs("-BEC=11 -bar=12");
    BOOST_CHECK_EQUAL(GetArg("-BEC", 0), 11);
    BOOST_CHECK_EQUAL(GetArg("-bar", 11), 12);

    ResetArgs("-BEC=NaN -bar=NotANumber");
    BOOST_CHECK_EQUAL(GetArg("-BEC", 1), 0);
    BOOST_CHECK_EQUAL(GetArg("-bar", 11), 0);
}

BOOST_AUTO_TEST_CASE(doubledash)
{
    ResetArgs("--BEC");
    BOOST_CHECK_EQUAL(GetBoolArg("-BEC"), true);

    ResetArgs("--BEC=verbose --bar=1");
    BOOST_CHECK_EQUAL(GetArg("-BEC", ""), "verbose");
    BOOST_CHECK_EQUAL(GetArg("-bar", 0), 1);
}

BOOST_AUTO_TEST_CASE(boolargno)
{
    ResetArgs("-noBEC");
    BOOST_CHECK(!GetBoolArg("-BEC"));
    BOOST_CHECK(!GetBoolArg("-BEC", true));
    BOOST_CHECK(!GetBoolArg("-BEC", false));

    ResetArgs("-noBEC=1");
    BOOST_CHECK(!GetBoolArg("-BEC"));
    BOOST_CHECK(!GetBoolArg("-BEC", true));
    BOOST_CHECK(!GetBoolArg("-BEC", false));

    ResetArgs("-noBEC=0");
    BOOST_CHECK(GetBoolArg("-BEC"));
    BOOST_CHECK(GetBoolArg("-BEC", true));
    BOOST_CHECK(GetBoolArg("-BEC", false));

    ResetArgs("-BEC --noBEC");
    BOOST_CHECK(GetBoolArg("-BEC"));

    ResetArgs("-noBEC -BEC"); // BEC always wins:
    BOOST_CHECK(GetBoolArg("-BEC"));
}

BOOST_AUTO_TEST_SUITE_END()
