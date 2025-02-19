nt main() {
    // srand(time(NULL));
    int n = 100;
    double a = rand() / double(RAND_MAX);
    double b = rand() / double(RAND_MAX);
    a = 10 + a * 10;
    b = 0.9 * a * b;
    double X[n], Y[n];
    for (int i = 0; i < n; i++) {
        X[i] = 10 * i / n;
        Y[i] = sin(2.0 * pi * (a * X[i] + b));
    }
    auto guess = Guess(n, X, Y);
    cout << "true a - guess a is " << a-guess.first << endl;
    cout << "true b - guess b is " << b-guess.second << endl;
    return 0;
}

