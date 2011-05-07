typedef pair<int,int> bezout;
bezout find_bezout(int x, int y) {
    if (y == 0) return bezout(1, 0);
    bezout u = find_bezout(y, x % y);
    return bezout(u.second, u.first - (x/y) * u.second);
}
