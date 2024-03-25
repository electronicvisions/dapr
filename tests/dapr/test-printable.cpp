#include "dapr/printable.h"

#include <sstream>
#include <gtest/gtest.h>


namespace {

struct DummyPrintable : public dapr::Printable
{
	int value;

	DummyPrintable(int value) : value(value) {}

protected:
	virtual std::ostream& print(std::ostream& os) const override
	{
		return os << "DummyPrintable(" << value << ")";
	}
};

} // namespace

TEST(Printable, General)
{
	DummyPrintable dummy_5(5);

	std::stringstream ss_operator;
	ss_operator << dummy_5;
	EXPECT_EQ(ss_operator.str(), "DummyPrintable(5)");
}
