/* Calcula centroide (centro de massa / gravidade) de poligono em O(n) */
double x[NMAX], y[NMAX];
x[n] = x[0];
y[n] = y[0];
double area = 0;
for (int i = 0; i < n; i++) {
	area+= x[i]*y[i+1] - y[i]*x[i+1];
}
double cx = 0, cy = 0;
for (int i = 0; i < n; i++) {
	cx+= (x[i] + x[i+1]) * (x[i]*y[i+1] - x[i+1]*y[i]);
	cy+= (y[i] + y[i+1]) * (x[i]*y[i+1] - x[i+1]*y[i]);
}
cx/= 3.0*area;
cy/= 3.0*area;
printf("%.2f %.2f\n", cx, cy);

