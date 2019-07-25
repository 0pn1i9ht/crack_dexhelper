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

#ifndef DECRYPT_H_
#define DECRYPT_H_


typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

struct ZipFileHeader {
	uint32_t magic;
	uint16_t version;
	uint16_t flags;
	uint16_t compression_method;
	uint16_t lastmodtime;
	uint16_t lastmoddate;
	uint32_t crc32_cs;
	uint32_t compress_size;
	uint32_t file_size;
	uint16_t file_name_length;
	uint16_t extra_field_len; // ��չ������
} __attribute__((packed));

#ifdef	__cplusplus
extern "C" {
#endif

void decrypt_classes0jar(const char* sfile, const char* dfile);

#ifdef	__cplusplus
}
#endif

#endif /* DECRYPT_H_ */
