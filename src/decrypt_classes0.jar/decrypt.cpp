/**
 * Author: sp00f
 * ��Ȩ�����Ҹ������У������ת�أ�����Ҫ��������
 * ���ĵ�ֻ����ѧϰ����Ŀ�ģ���������Ŀ�ı��˸Ų�����
 * ��ȫ���ǹ�����ϣ���������ܶԷ��ص�һ���ṩ����
 * ��������˼·ͬ�����죬�����������վ��ѧϰ�ߺ�̽��ӹ�ǿ�ȽǶȽ��У�û������κ��ض�����
 * ������İ汾�������°汾�������Ҷ���֪�����ĸ��汾�����Ҷ���ȡ��������������
 * ������²��ң���������ϣ�������ҵ�����ɹ����в��Եĵط�������ָ������һ���̽��
 * ����������Ҳ�Ǹ�Сѧ����ˮƽ���ޣ����������һЦ����
 *
*/

#include "decrypt.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>

#define LOBYTE(x)   (*((unsigned char*)&(x)))   // low byte

static int decrypt(int foff, unsigned char* buf, int buflen) {
	// �����㷨 foff�ļ�ƫ��С��0x20000��rc4����򣬴���ֱ�����0xac
	// �����rc4�㷨�о����в�ͬ�� ����Կ�̶��� ��κ���û���õ���ʱ����T
	int v3; // r6@2
	unsigned char* kbox; // r8@5
	int v6; // r4@7
	int j; // lr@7
	int i; // r5@7
	int tmp; // r9@8
	int k; // r12@8
	int v11; // lr@11
	int v12; // r5@11
	int v13; // r4@11
	unsigned char v14; // r12@13
	unsigned char* bufp; // lr@14
	unsigned char v16; // r0@15
	int v17; // r8@15
	int v18; // t1@15
	unsigned char* curr_bufp; // r1@19
	unsigned char sbox[256]; // [sp+4h] [bp-124h]@6

	unsigned char key[16] = {0x66,0x97,0x6C,0xE8, 0x6D, 0x46, 0x38,
					0xB0, 0x9, 0x5A, 0xA5, 0xD7, 0xF, 0xCB, 0x9A, 0xA0};

	if (foff >= 0x20000) {
		v3 = 0;
	} else { // foff < 0x20000

		v3 = 0x20000 - foff;
		if (0x20000 - foff >= buflen)
			v3 = buflen;

		buflen -= v3; // �����������û��

		if (v3 > 0) { // ��rc4
			kbox = key; //
		    for(int i = 0; i < 256; i++)//��ʼ���㷨
		    {
		        sbox[i] = i;
		    }

			v6 = 0;
			j = 0;
			i = 0;

			do { // ����256��
				tmp = sbox[i];
				k = kbox[v6++];
				j +=  (k + tmp);
				j = j %256;
				if (v6 > 15)
					v6 = 0;

				sbox[i++] = sbox[j];
				sbox[j] = tmp;
			} while (i != 256);

			v11 = 0;
			LOBYTE(v12) = 0;
			LOBYTE(v13) = 0;

			while (v11 != foff) { // �ڽ���foff��
				++v11;
				v13 =  (v13 + 1) % 256;
				v14 = sbox[v13];
				v12 = (v12 + v14) % 256;
				sbox[v13] = sbox[v12];
				sbox[v12] = v14;

			}

			//ǰ�涼���ڽ���
			bufp = buf;

			do { // �˴���ʼ��������
				v13 = (unsigned char) (v13 + 1);
				v16 = sbox[v13];
				v12 =  (unsigned char)(v12 + v16);
				sbox[v13] = sbox[v12];
				sbox[v12] = v16;

				v17 = (unsigned char) sbox[(unsigned char)(v16 + sbox[v13])];

				v18 = *(char*) bufp++;

				foff = v17 ^ v18;

				*(char*) (bufp - 1) = v17 ^ v18;

			} while ((int)bufp != ((int)buf + v3));

		}
	}

	if (buflen > 0) { // ���foff > 0x20000Ҳ����128k��ֱ��������
		curr_bufp =  buf + v3;
		foff = (int) curr_bufp; // buf ��ʼ��ַ

		do {
			*(unsigned char*) curr_bufp++ ^= 0xACu;
		} while ((int)curr_bufp - foff < buflen);
	}

	return foff; // ������ʼbuf��ַ
}


/**
 **ע������Linux����
 **��֮ǰ��windows mingw �� read���������
 **����ֻ�����дд
 **û���Ż�����Ϊ�����windows�±�д�Ĵ���
 **����û����mmap
 **�Գ���������exit����ʱҲ��Ϊ��ʡ��
 **����������Ż��öγ���
 */
void decrypt_classes0jar(const char* file, const char* ofile) {
	int fd = open(file, O_RDONLY);
	if (fd < 0) {
		perror("open file failed!");
		exit(1);
	}

	int fid = open(ofile, O_WRONLY | O_CREAT );
	if( fid < 0) {
		perror("creat zip file failed!");
		exit(1);
	}


	struct stat statbuf;
	stat(file, &statbuf);
	int size = statbuf.st_size;

	printf("file size = %x\n", size);

	char zheader[0x100];
	char zname[0x100];
	char zex[0x100];
	int loop_size = 1024;

	char* filebuf = (char*) malloc(size);
	if (!filebuf) {
		perror("malloc filebuf failed!");
		exit(-1);
	}

	memset(filebuf, 0, size);
	memset(zheader, 0, 0x100);
	memset(zname, 0, 0x100);
	memset(zex, 0, 0x100);

	int zheader_size = sizeof(struct ZipFileHeader);

	off_t off = lseek(fd, 0, SEEK_SET);
	printf("off = %x\n", off);
	ssize_t len = read(fd, zheader, zheader_size);
	printf("len = %lx\n", len);
	size_t f_off = decrypt(0, (unsigned char*)zheader, zheader_size);
	memcpy(filebuf, zheader, zheader_size);

	struct ZipFileHeader* zip_header = (struct ZipFileHeader*) zheader;

	int first_zipentry_off = (sizeof(struct ZipFileHeader) + zip_header->compress_size +
			zip_header->file_name_length + zip_header->extra_field_len);

	int buf2_size = size - first_zipentry_off;
	printf("first zipentry size = %x, buf2 size = %x\n", first_zipentry_off, buf2_size);

	char* buf1 =  (char*) malloc(loop_size);
	if (!buf1) {
		perror("malloc buf1 failed!");
		exit(-1);
	}

	char* buf2 = (char*) malloc(buf2_size);
	if (!buf2) {
		perror("malloc buf2 failed!");
		exit(-1);
	}

	memset(buf1, 0, loop_size);
	memset(buf2, 0, buf2_size);

	off = lseek(fd, 0, SEEK_CUR);
	printf("off = %x\n", off);
	len = read(fd, zname, zip_header->file_name_length);
	printf("len = %lx\n", len);
	f_off = decrypt(0, (unsigned char*)zname, zip_header->file_name_length);
	memcpy(filebuf + off, zname, zip_header->file_name_length);

	off = lseek(fd, 0, SEEK_CUR);
	printf("off = %x\n", off);
	len = read(fd, zex, zip_header->extra_field_len);
	printf("len = %lx\n", len);
	f_off = decrypt(0, (unsigned char*)zex, zip_header->extra_field_len);
	memcpy(filebuf + off, zex, zip_header->extra_field_len);

	off = lseek(fd, 0, SEEK_CUR);
	printf("off = %x\n", off);

	while (off < first_zipentry_off) {
		int max_size = loop_size;
		int last_size = first_zipentry_off - off;
		if ( last_size < loop_size ) {
			max_size = last_size;
		}

		printf("max size = %x\n", max_size);

		len = read(fd, buf1, max_size);
		printf("len = %lx\n", len);
		f_off = decrypt(off, (unsigned char*)buf1, max_size);

		memcpy(filebuf + off, buf1, max_size);
		memset(buf1, 0, loop_size);


		off = lseek(fd, 0, SEEK_CUR);
		printf("off = %x\n", off);

	}


	off = lseek(fd, 0, SEEK_CUR);
	printf("off = %x\n", off);


	len = read(fd, buf2, buf2_size);
	printf("len = %x\n", len);
	f_off = decrypt(off, (unsigned char*)buf2, buf2_size);


	printf("buf2 size = %x, off = %x\n", buf2_size, off);
	memcpy(filebuf + off, buf2, buf2_size);

	ssize_t wlen = write(fid, filebuf, size);
	printf("wlen = %x\n", wlen);

	free(buf1);
	free(buf2);
	free(filebuf);

	buf1 = NULL;
	buf2 = NULL;
	filebuf = NULL;

	close(fd);
	close(fid);
}
