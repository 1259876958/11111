#pragma once
#define _CRT_SECURE_NO_WARNINGS 1
typedef struct _ARGB {
	byte a;
	byte r;
	byte g;
	byte b;
}ARGB;
ARGB color2Argb(DWORD c) {
	ARGB res;
	res.r = (byte)c;
	res.g = (byte)(c >> 8);
	res.b = (byte)(c >> 16);
	res.a = (byte)(c >> 24);
	return res;
}
int** HB(IMAGE* src) {
	DWORD* psrc = GetImageBuffer(src);
	int w = src->getwidth();
	int h = src->getheight();
	int** a, i, j, b, c;
	a = (int**)malloc(sizeof(int*) * h);
	for (i = 0; i < h; ++i)
		a[i] = (int*)malloc(sizeof(int) * w);
	for (i = 0; i < h; ++i) {
		for (j = 0; j < w; ++j)
			a[i][j] = 0;
	}
	for (i = 0; i < w * h; ++i) {
		ARGB t = color2Argb(psrc[i]);
		byte arv = (t.r + t.g + t.b) / 3;
		b = i % w;
		c = (i - b) / w;
		if (arv < 200)
			a[c][b] = 1;
	}
	return a;
}
int* XX(IMAGE* src, int** bb) {
	DWORD* psrc = GetImageBuffer(src);
	int w = src->getwidth();
	int h = src->getheight();
	int* a, b, c = 0, d = 1, i, j, k, l = 0;
	a = (int*)malloc(sizeof(int) * 8);
	for (i = 0; i < h; ++i) {
		for (j = 0, k = 0; j < w; ++j) {
			if (bb[i][j] == 1 || bb[i][j] == 2)
				++k;
		}
		if (k > w * 3 / 4 && c == 0) {
			c = 1;
			b = i;
		}
		else if (k <= w * 3 / 4 && c == 1) {
			if (d == 0 || d == 1) {
				b = (b + i) / 2;
				a[l] = b;
				++l;
			}
			else if (d == 4)
				d = -1;
			c = 0;
			++d;
		}
	}
	return a;
}
void BX(IMAGE* src, int** dd) {
	DWORD* psrc = GetImageBuffer(src);
	int w = src->getwidth();
	int h = src->getheight();
	int i, j = 0, k = 0, m;
	for (i = 0, j = 0; i < h * w; ++i) {
		for (m = 0; dd[m][0] != -1; ++m) {
			if ((k - dd[m][1]) * (k - dd[m][1]) + (j - dd[m][2]) * (j - dd[m][2]) <= dd[m][3] * dd[m][3]
				 && dd[m][5]==1)
				psrc[i] = RGB(0, 255, 255);
		}
		if (j < w - 1)
			++j;
		else {
			j = 0;
			++k;
		}
	}
}
void HH(int w, int h, int** bb,int *d,int **f) {
	int i, j, k, l, m=0, n, o, a, c, e, r,p0,p1,p2,p3,p4,p5;
	double A,B,C,D;
	for (r = 7; r > 6; --r) {
		for (i = r+1; i < h-r-1; ++i) {
			for (j = r; j < w-r; ++j) {
				for (k = r, e = 0; k >= -r; --k) {
					l = (int)pow(r * r - k * k, 0.5);
					for (c = l; c >= -l; --c) {
						if (bb[i + c][j - k] != 1)
							e = 1;
					}
				}
				if (e == 0) {
					if (i < (d[3] + d[4]) / 2) {
						if (i < (d[1] + d[2]) / 2) {
							A = 8. * (d[1] - i) / (d[1] - d[0])+ 100.;
							a = (int)(A + 0.5);
						}
						else {
							B = 8. * (d[2] - i) / (d[3] - d[2])- 100.;
							a = (int)(B - 0.5);
						}
					}
					else {
						f[m][0] = 1;
						if (i < (d[5] + d[6]) / 2) {
							C = 8.*(d[5] - i) / (d[5] - d[4]) + 100.;
							a = (int)(C + 0.5);
						}
						else {
							D = 8. * (d[6] - i) / (d[7] - d[6]) - 100.;
							a = (int)(D - 0.5);
						}
					}
					f[m][1] = i;
					f[m][2] = j;
					f[m][3] = r;
					f[m][4] = a;
					++m;
					for (k = r; k >= -r; --k) {
						l = (int)pow(r * r - k * k, 0.5);
						for (c = l; c >= -l; --c)
							bb[i + c][j - k] = a;
					}
				}
			}
		}
	}
	f[m][0] = -1;
	for (n = 0; n < m-1; ++n) {
		for (o = 1; n + o < m; ++o) {
			if (f[n][0] > f[n + o][0] || f[n][0] == f[n + o][0] && f[n][2] > f[n + o][2]) {
				p0 = f[n][0];
				p1 = f[n][1];
				p2 = f[n][2];
				p3 = f[n][3];
				p4 = f[n][4];
				p5 = f[n][5];
				f[n][0] = f[n + o][0];
				f[n][1] = f[n + o][1];
				f[n][2] = f[n + o][2];
				f[n][3] = f[n + o][3];
				f[n][4] = f[n + o][4];
				f[n][5] = f[n + o][5];
				f[n + o][0] = p0;
				f[n + o][1] = p1;
				f[n + o][2] = p2;
				f[n + o][3] = p3;
				f[n + o][4] = p4;
				f[n + o][5] = p5;
			}
		}
	}
}
int BB(int **dd,int p,int a) {
	int i;
	if (dd[p][5] == -1) 
		return -1;
	else if (dd[p][4] == a && dd[p][5] == 0) {
		dd[p][5] = 1;
		return p + 1;
	}
	else {
		for (i = 1; p - i >= 0 && i <= 10; ++i) {
			if (dd[p - i][5] == 0 && dd[p - i][4] == a) {
				dd[p - i][5] = 1;
				return p - i+1;
			}
		}
	}
	for (i = 1; i <= 10; ++i) {
		if (dd[p + i][4] == a && dd[p + i][5] == 0) {
			dd[p + i][5] = 1;
			return p + i+1;
		}
	}
	return -1;
}