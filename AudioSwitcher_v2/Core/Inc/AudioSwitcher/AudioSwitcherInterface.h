#pragma once

namespace AudioSwitcher
{
	class AudioSwitcherInterface
	{
	public:
		///true = commutated; false = nc
		virtual void SetCommutationState(bool state = false) = 0;
		///true = commutated; false = nc
		virtual bool GetCommutationState() = 0;
	};
}
