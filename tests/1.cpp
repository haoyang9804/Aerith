template <class T, class U>
struct is_same { static constexpr bool value = false; };

template <class T>
struct is_same<T, T> { static constexpr bool value = true; };

template <class T, class U>
concept same_as = is_same<T, U>::value;

int const& f();

// same_as<int const&> auto& i = f();