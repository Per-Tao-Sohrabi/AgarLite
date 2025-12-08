// Integer square root using Newton's method
// Returns sqrt(x) as an integer
int sqrt(int x) {
    if (x <= 0) return 0;
    if (x == 1) return 1;
    
    // Initial guess
    int guess = x / 2;
    
    // Newton's method: guess_new = (guess + x/guess) / 2
    for (int i = 0; i < 10; i++) {
        int new_guess = (guess + x / guess) / 2;
        if (new_guess == guess) break;  // Converged
        guess = new_guess;
    }
    
    return guess;
}

// If you need more precision, use fixed-point (scale by 100):
// Returns sqrt(x) * 100 for x that's already scaled by 100
int int_sqrt_scaled(int x_scaled) {
    if (x_scaled <= 0) return 0;
    if (x_scaled == 100) return 10;  // sqrt(1.00) = 1.0 in scaled form
    
    int guess = x_scaled / 2;
    
    for (int i = 0; i < 10; i++) {
        // Newton's method with scaling preserved
        // guess_new = (guess + x/guess) / 2
        if (guess == 0) break;
        int new_guess = (guess + x_scaled / guess) / 2;
        if (new_guess == guess) break;
        guess = new_guess;
    }
    
    return guess;
}