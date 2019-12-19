#pragma once
#include <functional>

namespace Periph
{
	class IIndicators
	{
	public:
		std::function<void(bool state)> LEDWriteState = 0;
		std::function<void(bool state)> AMPWriteState = 0;
	};
}
