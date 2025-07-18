#pragma once

class Verify
{
public:
	static void Update(std::string_view message, int value);

	template<typename T>
	static void Update(std::string_view message, T* var)
	{
		if (!var)
		{
			auto trace = std::to_string(std::stacktrace::current());
			throw std::runtime_error(message.data() + trace);
		}
	}
};

