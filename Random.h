#pragma once

#include <random>

class RandomManager
{
public:
	RandomManager(void) = default;
	~RandomManager(void) = default;

	#if __cplusplus >= 201703L // C++17 이상이면 if constexpr 사용
	template <typename Type>
	Type Random(void)
	{
		if constexpr (std::is_integral<Type>::value)
		{
			std::uniform_int_distribution<Type> distributor;
			return distributor(generator);
		}
		else if constexpr (std::is_floating_point<Type>::value)
		{
			std::uniform_real_distribution<Type> distributor;
			return distributor(generator);
		}
		else
			static_assert(std::is_arithmetic<Type>::value, "지원하지 않는 타입");
	}

	template <typename Type>
	Type Random(Type min, Type max)
	{
		if constexpr (std::is_integral<Type>::value)
		{
			std::uniform_int_distribution<Type> distributor(min, max);
			return distributor(generator);
		}
		else if constexpr (std::is_floating_point<Type>::value)
		{
			std::uniform_real_distribution<Type> distributor(min, max);
			return distributor(generator);
		}
		else
			static_assert(std::is_arithmetic<Type>::value, "지원하지 않는 타입");
	}
	template <typename Type>
	static Type GetRandom(void)
	{
		static std::random_device randomDevice;
		static std::mt19937 generator(randomDevice());

		if constexpr (std::is_integral<Type>::value)
		{
			std::uniform_int_distribution<Type> distributor;
			return distributor(generator);
		}
		else if constexpr (std::is_floating_point<Type>::value)
		{
			std::uniform_real_distribution<Type> distributor;
			return distributor(generator);
		}
		else
			static_assert(std::is_arithmetic<Type>::value, "지원하지 않는 타입");
	}

	template <typename Type>
	static Type GetRandom(Type min, Type max)
	{
		static std::random_device randomDevice;
		static std::mt19937 generator(randomDevice());

		if constexpr (std::is_integral<Type>::value)
		{
			std::uniform_int_distribution<Type> distributor(min, max);
			return distributor(generator);
		}
		else if constexpr (std::is_floating_point<Type>::value)
		{
			std::uniform_real_distribution<Type> distributor(min, max);
			return distributor(generator);
		}
		else
			static_assert(std::is_arithmetic<Type>::value, "지원하지 않는 타입");
	}
	#else // C++11/14 전용 (SFINAE)
	template <typename Type>
	typename std::enable_if<std::is_integral<Type>::value, Type>::type Random(Type min, Type max)
	{
		std::uniform_int_distribution<Type> distributor(min, max);
		return distributor(generator);
	}
	template <typename Type>
	typename std::enable_if<std::is_floating_point<Type>::value, Type>::type Random(Type min, Type max)
	{
		std::uniform_real_distribution<Type> distributor(min, max);
		return distributor(generator);
	}

	template <typename Type>
	static typename std::enable_if<std::is_integral<Type>::value, Type>::type GetRandom(Type min, Type max)
	{
		static std::random_device randomDevice;
		static std::mt19937 generator(randomDevice());
		std::uniform_int_distribution<Type> distributor(min, max);
		return distributor(generator);
	}
	template <typename Type>
	static typename std::enable_if<std::is_floating_point<Type>::value, Type>::type GetRandom(Type min, Type max)
	{
		static std::random_device randomDevice;
		static std::mt19937 generator(randomDevice());
		std::uniform_real_distribution<Type> distributor(min, max);
		return distributor(generator);
	}
	#endif

private:
	std::random_device randomDevice{};
	std::mt19937 generator{ randomDevice };
};