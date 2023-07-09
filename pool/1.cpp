
template<typename> 
concept C1 = true;

template<typename>
concept C2 = C1<int> && true;

template<typename>
concept C3 = true && C2<int>;

int main() { return 0; }