float sqrtf(float x) {
    if (x <= 0.0f) return 0.0f;
    
    float guess = x / 2.0f;
    for (int i = 0; i < 10; i++) {
        guess = (guess + x / guess) / 2.0f;
    }
    return guess;
}

double sqrt(double x) {
    if (x <= 0.0) return 0.0;
    
    double guess = x / 2.0;
    for (int i = 0; i < 10; i++) {
        guess = (guess + x / guess) / 2.0;
    }
    return guess;
}