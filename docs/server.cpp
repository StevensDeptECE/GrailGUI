int main(int argc, char* argv[]) {
    constexpr int N = 1024;
    DynArray a(N);
    for (int i = 0; i < N/2; i++)
      a.add_end(i);
    a.send_additions(0); // send additions since start
    for (int i = N/2; i < N; i++)
      a.add_end(i);
    a.send_additions(N/2); // send additions since N/2
    a.add_end(99);
    a.send_additions(N); // send single new element
}