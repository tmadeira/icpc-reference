llu mul_mod(llu a, llu b, llu m) {
    llu y = (llu)((ldouble)a*(ldouble)b/m+(ldouble)1/2); y = y * m;
    llu x = a * b, r = x - y;
    if ((lld) r < 0) { r = r + m; y = y - 1; }
    return r;
}
