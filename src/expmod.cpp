llu exp_mod(llu a, llu e, llu mod) {
    if (e == 0) return 1;
    llu b = exp_mod(a, e/2, mod);
    return (e % 2 == 0) ? mul_mod(b, b, mod) : mul_mod(mul_mod(b, b, mod), a, mod);
}
