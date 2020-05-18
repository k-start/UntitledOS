#include <stdlib.h>
#include <string.h>

void reverse(char s[]) {
    int c, i, j;
    for (i = 0, j = strlen(s)-1; i < j; i++, j--) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

void itoa(int n, char str[]) {
    int i, sign;
    if ((sign = n) < 0) n = -n;
    i = 0;
    do {
        str[i++] = n % 10 + '0';
    } while ((n /= 10) > 0);

    if (sign < 0) str[i++] = '-';
    str[i] = '\0';

    reverse(str);
}

char bchars[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
void __itoa(int i, unsigned int base, char buf[]) {
	int pos = 0;
	int opos = 0;
	int top = 0;

	if (i == 0 || base > 16) {
		buf[0] = '0';
		buf[1] = '\0';
		return;
	}

	while (i != 0) {
		buf[pos] = bchars[i % base];
		pos++;
		i /= base;
	}
    buf[pos] = '\0';
	reverse(buf);
}